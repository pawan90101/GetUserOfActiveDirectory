
#include "activedirectoryusers.h"

CActiveDirectoryUsers::CActiveDirectoryUsers(void)
{
}

CActiveDirectoryUsers::~CActiveDirectoryUsers(void)
{
}

bool CActiveDirectoryUsers::GetADUsers(vector<CString> & UserArray)
{
	//  Initialize COM.
	CoInitialize(NULL);
	HRESULT hr = S_OK;
	// Get rootDSE and the current user domain container distinguished name.
	IADs *pObject = NULL;
	IDirectorySearch *pContainerToSearch = NULL;
	LPOLESTR szPath = new OLECHAR[MAX_PATH];
	VARIANT var;
	hr = ADsOpenObject(_T("LDAP://rootDSE"),NULL,NULL,ADS_SECURE_AUTHENTICATION,IID_IADs,(void**)&pObject);//// Use Secure Authentication.
	if (FAILED(hr))
	{
		
		if (pObject)
			pObject->Release();
		return false;
	}
	if (SUCCEEDED(hr))
	{
		hr = pObject->Get(_T("defaultNamingContext"),&var);
		if (SUCCEEDED(hr))
		{
			//  Build path to the domain container.
			_tcscpy(szPath,_T("LDAP://"));
			_tcscat(szPath,var.bstrVal);
			hr = ADsOpenObject(szPath,NULL,	NULL,ADS_SECURE_AUTHENTICATION,	IID_IDirectorySearch,(void**)&pContainerToSearch);

			if (SUCCEEDED(hr))
			{
				bool success=FindUsers(&UserArray,pContainerToSearch);
				if (!success)
				{
					return false;					
				}
				
			}
			else
			{
				return false;					
			}
			if (pContainerToSearch)
				pContainerToSearch->Release();
		}
		VariantClear(&var);
	}
	if (pObject)
		pObject->Release();

	
	CoUninitialize();
	delete[] szPath;
	return true;
}

bool CActiveDirectoryUsers::FindUsers(vector<CString>* pUserArray,IDirectorySearch *pContainerToSearch)
{
	if(pContainerToSearch==NULL||pUserArray==NULL)
		return false;

	// Create search filter.
	LPOLESTR pszSearchFilter = new OLECHAR[MAX_PATH];
	
	//  Add the filter.
	//_tcscpy(pszSearchFilter,_T("(&(objectClass=user)(objectCategory=person))"));
	_tcscpy(pszSearchFilter, _T("(objectCategory=computer)"));

	//  Specify subtree search.
	ADS_SEARCHPREF_INFO SearchPrefs;
	SearchPrefs.dwSearchPref = ADS_SEARCHPREF_SEARCH_SCOPE;
	SearchPrefs.vValue.dwType = ADSTYPE_INTEGER;
	SearchPrefs.vValue.Integer = ADS_SCOPE_SUBTREE;
	DWORD dwNumPrefs = 1;


	//  COL for iterations.
	LPOLESTR pszColumn = NULL;    
	ADS_SEARCH_COLUMN col;
	HRESULT hr;

	//  Interface Pointers
	IADs    *pObj = NULL;
	IADs	* pIADs = NULL;

	//  Search handle.
	ADS_SEARCH_HANDLE hSearch = NULL;

	//  Set search preference.
	hr = pContainerToSearch->SetSearchPreference( &SearchPrefs, dwNumPrefs);
	if (FAILED(hr))
		return false;


	LPOLESTR pszNonVerboseList[] = {_T("name") , _T("distinguishedName" )};
	LPOLESTR szName = new OLECHAR[MAX_PATH];
	LPOLESTR szDN = new OLECHAR[MAX_PATH];

	int iCount = 0;

	//  Return non-verbose list properties only.
	hr = pContainerToSearch->ExecuteSearch(pszSearchFilter,	pszNonVerboseList,sizeof(pszNonVerboseList)/sizeof(LPOLESTR),&hSearch);
	if ( SUCCEEDED(hr) )
	{    
		//  Call IDirectorySearch::GetNextRow() to retrieve the next data row.
		hr = pContainerToSearch->GetFirstRow( hSearch);
		if (SUCCEEDED(hr))
		{
			while( hr != S_ADS_NOMORE_ROWS )
			{
				//  Keep track of count.
				iCount++;


				//  Loop through the array of passed column names,
				//  print the data for each column.
				while( pContainerToSearch->GetNextColumnName( hSearch, &pszColumn ) != S_ADS_NOMORE_COLUMNS )
				{
					hr = pContainerToSearch->GetColumn( hSearch, pszColumn, &col );
					if ( SUCCEEDED(hr) )
					{
						if (0 ==_tcscmp(_T("name"), pszColumn))
						{
							_tcscpy(szName,col.pADsValues->CaseIgnoreString);
#ifdef _UNICODE
							wstring temp = OLE2T(szName);
#else
							string temp = OLE2T(szName);
#endif
							pUserArray->push_back(temp.c_str());
							//_tprintf(_T("%s \n"), szName);
						}
						if (0 == wcscmp(L"distinguishedName", pszColumn))
						{
							_tcscpy(szDN, col.pADsValues->CaseIgnoreString);
							//_tprintf(_T("%s \n"), szDN);
						}

						pContainerToSearch->FreeColumn( &col );
					}
					FreeADsMem( pszColumn );
				}  

				 
				hr = pContainerToSearch->GetNextRow( hSearch);
			} 

		}
		else
			return false;
		 
		pContainerToSearch->CloseSearchHandle(hSearch);
	} 
	else
		return false;
	 

	delete[] szName;
	delete[] pszSearchFilter;

	return true;
}

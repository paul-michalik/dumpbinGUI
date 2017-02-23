// dumpbinGUI v 1.1.0.1
// Copyright(c) 2000-2003 tim tabor.  
// All rights reserved.  No Warranties extended.

#pragma once

// most definitely *not* a general purpose cmd line parser
class CCmdArgs
{
private:
	LPTSTR m_pmszArgs;
	LPCTSTR m_pszNext;

public:
	
	CCmdArgs() : 
		m_pmszArgs(0),
		m_pszNext(0)
	{}

	~CCmdArgs()
	{
		if (m_pmszArgs)
		{
			delete [] m_pmszArgs;
			m_pmszArgs = 0;
		}
	}

	bool Parse()
	{
		LPCTSTR pszArgs = GetCommandLine();
		if (!pszArgs) return false;

		int nLen = lstrlen(pszArgs) + 2;
		ATLTRY(m_pmszArgs = new TCHAR[nLen]);
		if (!m_pmszArgs) return false;

		ZeroMemory(m_pmszArgs, nLen * sizeof(TCHAR));

		bool bInQuote = false;
		for (int i=0, j=0; pszArgs[i]; ++i)
		{
			while (pszArgs[i] == _T(' ') && pszArgs[i+1] == _T(' '))
			{
				++i;
			}

			if (pszArgs[i] == _T('\"'))
			{
				bInQuote = !bInQuote;
			}

			m_pmszArgs[j++] = (!bInQuote && (pszArgs[i] == _T(' '))) ? 0 : pszArgs[i];
		}

		m_pszNext = m_pmszArgs;

		return true;
	}

	LPCTSTR Next()
	{
		if (!m_pszNext || !(*m_pszNext)) 
			return 0;

		LPCTSTR p = m_pszNext;
		m_pszNext += lstrlen(m_pszNext) + 1;

		return p;
	}
};

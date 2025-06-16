#pragma once

class HKEY_Helper
{
    HKEY m_hKey;
public:
    HKEY_Helper() = delete;
    HKEY_Helper(HKEY_Helper&) = delete;
    HKEY_Helper& operator=(const HKEY_Helper&) = delete;

    HKEY_Helper( const wchar_t* subKey,
                 const REGSAM samDesired = KEY_READ ) :
        m_hKey {}
    {
        const auto ec = ::RegOpenKeyExW(HKEY_LOCAL_MACHINE,
                                        subKey,
                                        0,
                                        samDesired,
                                        &m_hKey);
        if(ec != ERROR_SUCCESS)
        {
            System::UnicodeString msg {  "Cannot open " };
            msg += subKey;
            throw Exception { msg };
        }
    }

    ~HKEY_Helper()
    {
        ::RegCloseKey(m_hKey);
    }

    operator HKEY() const
    {
        return m_hKey;
    }
};
#include <QNetworkInterface>
#include <QStringList>
#include <stdio.h>

#ifdef WIN32
    #include <winsock2.h>
    #include <iphlpapi.h>
    #include <stdlib.h>
    #include <conio.h>

    #define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
    #define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
#endif

/******************************************************************/
/*                      GET_MAC_ADDRESS                            */
/******************************************************************/

QStringList GET_MAC_ADDRESS()
{
    QStringList      text_mac_address;

    #ifdef LINUX
        QList<QNetworkInterface> my_interface = QNetworkInterface::allInterfaces();
        for ( int i = 0 ; i < my_interface.size() ; i++ ) {
            if ( text_mac_address.contains( my_interface.at(i).hardwareAddress()) == false &&
                 my_interface.at(i).hardwareAddress().isEmpty()                   == false &&
                 (my_interface.at(i).flags() & QNetworkInterface::IsLoopBack )    != QNetworkInterface::IsLoopBack ) {
                text_mac_address << my_interface.at(i).hardwareAddress();
            }
        }
    #endif

    #ifdef WIN32
        PIP_ADAPTER_INFO pAdapter            = NULL;
        PIP_ADAPTER_INFO pAdapterInfo;
        DWORD            dwRetVal            = 0;

        ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
        pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(sizeof (IP_ADAPTER_INFO));
        if (pAdapterInfo == NULL) {
            printf("Error allocating memory needed to call GetAdaptersinfo\n");
        }

        if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
            FREE(pAdapterInfo);
            pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(ulOutBufLen);
            if (pAdapterInfo == NULL) {
                printf("Error allocating memory needed to call GetAdaptersinfo\n");
            }
        }

        // GetAdaptersInfo tum ag donanim bilgilerini alir
        // Burda filtreleme yapilmadi tum donanimlari okuduk gerekirse filtreleme yapilmali.
        if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
            pAdapter = pAdapterInfo;
            while (pAdapter) {
                QNetworkInterface my_interface = QNetworkInterface::interfaceFromName(pAdapter->AdapterName);
                switch (pAdapter->Type) {
                case MIB_IF_TYPE_ETHERNET:                    
                    text_mac_address << my_interface.hardwareAddress();
                    break;
                default:
                    text_mac_address << my_interface.hardwareAddress();
                    break;
                }
                pAdapter = pAdapter->Next;
            }
        }
        if (pAdapterInfo) {
            FREE(pAdapterInfo);
        }
    #endif

    return text_mac_address;;
}

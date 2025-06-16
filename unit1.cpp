//------------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "HKEY_Helper.h"
#include "Unit1.h"
#include "PasspR.h"  // Добавляем заголовочный файл SDK Regula
#include <Jpeg.hpp>  // Добавляем поддержку JPEG
#include <IOUtils.hpp> // Для TDirectory
#include <System.Zip.hpp> // Для работы с ZIP архивами в VCL

#include <array>
#include <algorithm>
#include <System.StrUtils.hpp>
#include <thread>
#include <chrono>

//------------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TForm1 *Form1;

//------------------------------------------------------------------------------

// Get reader name by its id
inline UnicodeString __fastcall GetDeviceNameByID( const uint32_t id )
{
    static const std::array< UnicodeString, 93 > kDeviceTypes
    {
        "Unknown",                    //0
        "7004S",
        "7003.01",
        "7003.110",
        "7003.111",
        "7004.110",
        "70x4.111",
        "70x4.114",
        "70x4.115",
        "7004.100",
        "8303.100",                   //10
        "8303.110",
        "8303.111",
        "8303.114",
        "7104.115",
        "7304.115",
        "7504.115",
        "7384.115",
        "8303.115",
        "8313.115",
        "8333.115",                   //20
        "8353.115",
        "8383.115",
        "7103.115",
        "7303.115",
        "7503.115",
        "7383.115",
        "8314.115",
        "8334.115",
        "8354.115",
        "8384.115",                   //30
        "8305",
        "7107.115",
        "7307.115",
        "7507.115",
        "7387.115",
        "8307",
        "4820 (OV 1.3 Mp)",
        "4820 (OV 3.1 Mp)",
        "4820 (OV 5.1 Mp)",
        "4820 (Micron 3.1 Mp)",       //40
        "4820 (Micron 9.0 Mp)",

        "Virtual Reader",

        "7117.115",
        "7317.115",
        "7517.115",
        "7397.115",                   // 46

        "7024 (Micron 9.0 Mp)",
        "8303 (Micron 9.0 Mp)",
        "8304 (Micron 9.0 Mp)",
        "7007 (Micron 9.0 Mp)",       // 50
        "7003 (Micron 9.0 Mp)",

        "7074",                       // 52
        "7084",                       // 53

        // micron 5 MP
        "8853.115",                   // 54
        "8854.115",                   // 55
        "7857.115",                   //
        "7854.115",                   //
        "7853.115",                   //
        "4858",                       // 59

        "MMM",                         //60
        "7038.115 (Micron 3.1)",      // 61 - Micron 3.1
        "7058.115 (Micron 5.0)",      // 62 - Micron 5.0

        "7308.115 (Micron 3.1)",      // 63 - Micron 3.1
        "78E4 (Micron 14.0)",         // 64 - Micron 14.0
        "70x4.115M (Micron 3.1)",     // 65 - Micron 3.1
        "70x4.115M (Micron 5.0)",     // 66 - Micron 5.0

        "70x4.115M (Micron 10.0)",    // 67 - Micron 10.0
        "78A4 (Micron 10.0)",         // 68 - Micron 10.0
        "7303 (Blackfin)",            // 69
        "7303 (Cypress)",             // 70
        "E_SEEK M-500",               // 71
        "TWAIN scanner",              // 72
        "72x3 (OV 5.1 Mp)",           // 73 RPRM_DeviceType_FX_7253
        "7017 (Micron 5Mp)",          // 74 RPRM_DeviceType_FX_7517
        "7017 (OV 5Mp)",              // 75 RPRM_DeviceType_FX_7017
        "7027 (OV 5Mp)",              // 76 RPRM_DeviceType_FX_7027
        "8303M.115",                  // 77 RPRM_DeviceType_FX_8303_115
        "8803",                       // 78 RPRM_DeviceType_EOS_8803_100
        "8850",                       // 79 RPRM_DeviceType_EOS_8850_5
        "8824 80",                    // 80 RPRM_DeviceType_EOS_8824_80
        "70x8M (OV 5 Mp)",            // 81 RPRM_DeviceType_FX_7058M
        "70x8M (OV 5 Mp, VB)",        // 82 RPRM_DeviceType_FX_7058M_VB
        "7017D Master",               // 83 RPRM_DeviceType_FX_7017D_M
        "7017D Slave",                // 84 RPRM_DeviceType_FX_7017D_S
        "7038.115 (Micron 3.1, VB)",  // 85 RPRM_DeviceType_FX_7038_VB
        "70x4M.115-5A",               // 86 RPRM_DeviceType_FX_76x4M
        "8850M",                      // 87 RPRM_DeviceType_EOS_8850M_9
        "Panasonic-based",            // 88 RPRM_DeviceType_Panasonic
        "7310 Mobile complex",        // 89 RPRM_DeviceType_Mobile_7310
        "70x4.115M (Micron 18.0)",    // 90 - Micron 18.0
        "70x8M (AR 13 Mp)",           // 91 - RPRM_DeviceType_FX_7038M
        "DESKO Penta Scanner",        // 92 - RPRM_DeviceType_Desko
    }; // kDeviceTypes

    switch( id )
    {
    case RPRM_DeviceType_USB20_1:      return kDeviceTypes[ 0 ];
    case RPRM_DeviceType_7004s:        return kDeviceTypes[ 1 ];
    case RPRM_DeviceType_7003_01:      return kDeviceTypes[ 2 ];
    case RPRM_DeviceType_7003_110:     return kDeviceTypes[ 3 ];
    case RPRM_DeviceType_7003_111:     return kDeviceTypes[ 4 ];
    case RPRM_DeviceType_7004_110:     return kDeviceTypes[ 5 ];
    case RPRM_DeviceType_70x4_111:     return kDeviceTypes[ 6 ];
    case RPRM_DeviceType_70x4_114:     return kDeviceTypes[ 7 ];
    case RPRM_DeviceType_70x4_115:     return kDeviceTypes[ 8 ];
    case RPRM_DeviceType_7004_100:     return kDeviceTypes[ 9 ];
    case RPRM_DeviceType_8303_100:     return kDeviceTypes[ 10 ];
    case RPRM_DeviceType_8303_110:     return kDeviceTypes[ 11 ];
    case RPRM_DeviceType_8303_111:     return kDeviceTypes[ 12 ];
    case RPRM_DeviceType_8303_114:     return kDeviceTypes[ 13 ];
    case RPRM_DeviceType_FX_7104_Lite: return kDeviceTypes[ 14 ];
    case RPRM_DeviceType_FX_7104_115:  return kDeviceTypes[ 14 ];
    case RPRM_DeviceType_FX_7304_Lite: return kDeviceTypes[ 15 ];
    case RPRM_DeviceType_FX_7304_115:  return kDeviceTypes[ 15 ];
    case RPRM_DeviceType_FX_7504_Lite: return kDeviceTypes[ 16 ];
    case RPRM_DeviceType_FX_7504_115:  return kDeviceTypes[ 16 ];
    case RPRM_DeviceType_FX_7384_Lite: return kDeviceTypes[ 17 ];
    case RPRM_DeviceType_FX_7384_115:  return kDeviceTypes[ 17 ];
    case RPRM_DeviceType_8303_115:     return kDeviceTypes[ 18 ];
    case RPRM_DeviceType_FX_8313_115:  return kDeviceTypes[ 19 ];
    case RPRM_DeviceType_FX_8333_115:  return kDeviceTypes[ 20 ];
    case RPRM_DeviceType_FX_8353_115:  return kDeviceTypes[ 21 ];
    case RPRM_DeviceType_FX_8383_115:  return kDeviceTypes[ 22 ];
    case RPRM_DeviceType_FX_7103_115:  return kDeviceTypes[ 23 ];
    case RPRM_DeviceType_FX_7303_115:  return kDeviceTypes[ 24 ];
    case RPRM_DeviceType_FX_7503_115:  return kDeviceTypes[ 25 ];
    case RPRM_DeviceType_FX_7383_115:  return kDeviceTypes[ 26 ];
    case RPRM_DeviceType_FX_8314_115:  return kDeviceTypes[ 27 ];
    case RPRM_DeviceType_FX_8334_115:  return kDeviceTypes[ 28 ];
    case RPRM_DeviceType_FX_8354_115:  return kDeviceTypes[ 29 ];
    case RPRM_DeviceType_FX_8384_115:  return kDeviceTypes[ 30 ];
    case RPRM_DeviceType_8305:         return kDeviceTypes[ 31 ];
    case RPRM_DeviceType_FX_7107_115:  return kDeviceTypes[ 32 ];
    case RPRM_DeviceType_FX_7307_115:  return kDeviceTypes[ 33 ];
    case RPRM_DeviceType_FX_7507_115:  return kDeviceTypes[ 34 ];
    case RPRM_DeviceType_FX_7387_115:  return kDeviceTypes[ 35 ];
    case RPRM_DeviceType_FX_8307:      return kDeviceTypes[ 36 ];
    case RPRM_DeviceType_FX_4821:      return kDeviceTypes[ 37 ];
    case RPRM_DeviceType_FX_4823:      return kDeviceTypes[ 38 ];
    case RPRM_DeviceType_FX_4825:      return kDeviceTypes[ 39 ];
    case RPRM_DeviceType_FX_4822:      return kDeviceTypes[ 40 ];
    case RPRM_DeviceType_FX_4828:      return kDeviceTypes[ 41 ];
    case RPRM_DeviceType_Virtual:      return kDeviceTypes[ 42 ];
    case RPRM_DeviceType_FX_7117_115:  return kDeviceTypes[ 43 ];
    case RPRM_DeviceType_FX_7317_115:  return kDeviceTypes[ 44 ];
    case RPRM_DeviceType_FX_7517_115:  return kDeviceTypes[ 45 ];
    case RPRM_DeviceType_FX_7397_115:  return kDeviceTypes[ 46 ];
    case RPRM_DeviceType_FX_7884_Lite: return kDeviceTypes[ 47 ];
    case RPRM_DeviceType_FX_7884_115:  return kDeviceTypes[ 47 ];
    case RPRM_DeviceType_FX_8883_115:  return kDeviceTypes[ 48 ];
    case RPRM_DeviceType_FX_8884_115:  return kDeviceTypes[ 49 ];
    case RPRM_DeviceType_FX_7887_115:  return kDeviceTypes[ 50 ];
    case RPRM_DeviceType_FX_7883_115:  return kDeviceTypes[ 51 ];
    case RPRM_DeviceType_EOS_7074_550: return kDeviceTypes[ 52 ];
    case RPRM_DeviceType_EOS_7084_7:   return kDeviceTypes[ 53 ];
    case RPRM_DeviceType_FX_8853_115:  return kDeviceTypes[ 54 ];
    case RPRM_DeviceType_FX_8854_115:  return kDeviceTypes[ 55 ];
    case RPRM_DeviceType_FX_7857_115:  return kDeviceTypes[ 56 ];
    case RPRM_DeviceType_FX_7854_115:  return kDeviceTypes[ 57 ];
    case RPRM_DeviceType_FX_7854_Lite: return kDeviceTypes[ 57 ];
    case RPRM_DeviceType_FX_7853_115:  return kDeviceTypes[ 58 ];
    case RPRM_DeviceType_FX_4858:      return kDeviceTypes[ 59 ];
    case RPRM_DeviceType_3M:           return kDeviceTypes[ 60 ];
    case RPRM_DeviceType_FX_7038:      return kDeviceTypes[ 61 ];
    case RPRM_DeviceType_FX_7058:      return kDeviceTypes[ 62 ];
    case RPRM_DeviceType_FX_7338:      return kDeviceTypes[ 63 ];
    case RPRM_DeviceType_FX_78E4_115:  return kDeviceTypes[ 64 ];
    case RPRM_DeviceType_FX_78E4_Lite: return kDeviceTypes[ 64 ];
    case RPRM_DeviceType_FX_73x4M:     return kDeviceTypes[ 65 ];
    case RPRM_DeviceType_FX_75x4M:     return kDeviceTypes[ 66 ];
    case RPRM_DeviceType_FX_71x4M:     return kDeviceTypes[ 67 ];
    case RPRM_DeviceType_FX_78A4_115:  return kDeviceTypes[ 68 ];
    case RPRM_DeviceType_FX_78A4_Lite: return kDeviceTypes[ 68 ];
    case RPRM_DeviceType_73xx:         return kDeviceTypes[ 69 ];
    case RPRM_DeviceType_BK:           return kDeviceTypes[ 70 ];
    case RPRM_DeviceType_M500:         return kDeviceTypes[ 71 ];
    case RPRM_DeviceType_TWAIN:        return kDeviceTypes[ 72 ];
    case RPRM_DeviceType_FX_7253:      return kDeviceTypes[ 73 ];
    case RPRM_DeviceType_FX_7517:      return kDeviceTypes[ 74 ];
    case RPRM_DeviceType_FX_7017:      return kDeviceTypes[ 75 ];
    case RPRM_DeviceType_FX_7027:      return kDeviceTypes[ 76 ];
    case RPRM_DeviceType_FX_8303_115:  return kDeviceTypes[ 77 ];
    case RPRM_DeviceType_EOS_8803_100: return kDeviceTypes[ 78 ];
    case RPRM_DeviceType_EOS_8850_5:   return kDeviceTypes[ 79 ];
    case RPRM_DeviceType_EOS_8824_80:  return kDeviceTypes[ 80 ];
    case RPRM_DeviceType_FX_7058M:     return kDeviceTypes[ 81 ];
    case RPRM_DeviceType_FX_7058M_VB:  return kDeviceTypes[ 82 ];
    case RPRM_DeviceType_FX_7017D_M:   return kDeviceTypes[ 83 ];
    case RPRM_DeviceType_FX_7017D_S:   return kDeviceTypes[ 84 ];
    case RPRM_DeviceType_FX_7038_VB:   return kDeviceTypes[ 85 ];
    case RPRM_DeviceType_FX_76x4M:     return kDeviceTypes[ 86 ];
    case RPRM_DeviceType_EOS_8850M_9:  return kDeviceTypes[ 87 ];
    case RPRM_DeviceType_Panasonic:    return kDeviceTypes[ 88 ];
    case RPRM_DeviceType_Mobile_7310:  return kDeviceTypes[ 89 ];
    case RPRM_DeviceType_FX_78x4M:     return kDeviceTypes[ 90 ];
    case RPRM_DeviceType_FX_7038M:     return kDeviceTypes[ 91 ];
    case RPRM_DeviceType_Desko:        return kDeviceTypes[ 92 ];
    default:
        break;
    }
    return kDeviceTypes[ 0 ];
}

//------------------------------------------------------------------------------

inline
UnicodeString ErrorToStr( const eRPRM_ErrorCodes msg )
{
    UnicodeString str;
    switch( msg )
    {
    case RPRM_Error_NoError:
        str = L"Success";
        break;
    case RPRM_Error_AlreadyDone:
        str = L"RPRM_Error_AlreadyDone";
        break;
    case RPRM_Error_NoGraphManager:
        str = L"RPRM_Error_NoGraphManager";
        break;
    case RPRM_Error_CantRegisterMessages:
        str = L"RPRM_Error_CantRegisterMessages";
        break;
    case RPRM_Error_NoServiceManager:
        str = L"RPRM_Error_NoServiceManager";
        break;
    case RPRM_Error_CantConnectServiceManager:
        str = L"RPRM_Error_CantConnectServiceManager";
        break;
    case RPRM_Error_CantCreateDeviceLibraryEvent:
        str = L"RPRM_Error_CantCreateDeviceLibraryEvent";
        break;
    case RPRM_Error_InvalidParameter:
        str = L"RPRM_Error_InvalidParameter";
        break;
    case RPRM_Error_NotInitialized:
        str = L"RPRM_Error_NotInitialized";
        break;
    case RPRM_Error_Busy:
        str = L"RPRM_Error_Busy";
        break;
    case RPRM_Error_NotEnoughMemory:
        str = L"RPRM_Error_NotEnoughMemory";
        break;
    case RPRM_Error_BadVideo:
        str = L"RPRM_Error_BadVideo";
        break;
    case RPRM_Error_ScanAborted:
        str = L"RPRM_Error_ScanAborted";
        break;
    case RPRM_Error_CantRecognizeDocumentType:
        str = L"RPRM_Error_CantRecognizeDocumentType";
        break;
    case RPRM_Error_CantSetupSensor:
        str = L"RPRM_Error_CantSetupSensor";
        break;
    case RPRM_Error_NotTrueColorDesktop:
        str = L"RPRM_Error_NotTrueColorDesktop";
        break;
    case RPRM_Error_NotAvailable:
        str = L"RPRM_Error_NotAvailable";
        break;
    case RPRM_Error_DeviceError:
        str = L"RPRM_Error_DeviceError";
        break;
    case RPRM_Error_DeviceDisconnected:
        str = L"RPRM_Error_DeviceDisconnected";
        break;
    case RPRM_Error_WrongThreadContext:
        str = L"RPRM_Error_WrongThreadContext";
        break;
    case RPRM_Error_COMServers:
        str = L"RPRM_Error_COMServers";
        break;
    case RPRM_Error_NoDocumentReadersFound:
        str = L"RPRM_Error_NoDocumentReadersFound";
        break;
    case RPRM_Error_NoTranslationMngr:
        str = L"RPRM_Error_NoTranslationMngr";
        break;
    case RPRM_Error_NoActiveDevice:
        str = L"RPRM_Error_NoActiveDevice";
        break;
    case RPRM_Error_ConnectingDevice:
        str = L"RPRM_Error_ConnectingDevice";
        break;
    case RPRM_Error_Failed:
        str = L"RPRM_Error_Failed";
        break;
    case RPRM_Error_LightIsNotAllowed:
        str = L"RPRM_Error_LightIsNotAllowed";
        break;
    case RPRM_Error_ImageIOError:
        str = L"RPRM_Error_ImageIOError";
        break;
    case RPRM_Error_CantStoreCalibrationData:
        str = L"RPRM_Error_CantStoreCalibrationData";
        break;
    case RPRM_Error_DeviceNotCalibrated:
        str = L"RPRM_Error_DeviceNotCalibrated";
        break;
    case RPRM_Error_CantCompensateDistortion:
        str = L"RPRM_Error_CantCompensateDistortion";
        break;
    case RPRM_Error_OperationCancelled:
        str = L"RPRM_Error_OperationCancelled";
        break;
    case RPRM_Error_CantLocateDocumentBounds:
        str = L"RPRM_Error_CantLocateDocumentBounds";
        break;
    case RPRM_Error_CantRefineImages:
        str = L"RPRM_Error_CantRefineImages";
        break;
    case RPRM_Error_CantCropRotateImages:
        str = L"RPRM_Error_CantCropRotateImages";
        break;
    case RPRM_Error_IncompleteImagesList:
        str = L"RPRM_Error_IncompleteImagesList";
        break;
    case RPRM_Error_CantReadMRZ:
        str = L"RPRM_Error_CantReadMRZ";
        break;
    case RPRM_Error_CantFindBarcodes:
        str = L"RPRM_Error_CantFindBarcodes";
        break;
    case RPRM_Error_DeviceIDNotSupported:
        str = L"RPRM_Error_DeviceIDNotSupported";
        break;
    case RPRM_Error_DeviceIDNotStored:
        str = L"RPRM_Error_DeviceIDNotStored";
        break;
    case RPRM_Error_DeviceDriver:
        str = L"RPRM_Error_DeviceDriver";
        break;
    case RPRM_Error_Calibration_OpenLid:
        str = L"RPRM_Error_Calibration_OpenLid";
        break;
    case RPRM_Error_Calibration_Brightness:
        str = L"RPRM_Error_Calibration_Brightness";
        break;
    case RPRM_Error_Calibration_WhiteBalance:
        str = L"RPRM_Error_Calibration_WhiteBalance";
        break;
    case RPRM_Error_Calibration_TargetPosition:
        str = L"RPRM_Error_Calibration_TargetPosition";
        break;
    case RPRM_Error_Calibration_LightBlank:
        str = L"RPRM_Error_Calibration_LightBlank: blank spots detected";
        break;
    case RPRM_Error_Calibration_LightDistortion:
        str = L"RPRM_Error_Calibration_LightDistortion";
        break;
    case RPRM_Error_Calibration_LightLevel:
        str =
          L"RPRM_Error_Calibration_LightLevel: lighting level is unacceptable";
        break;
    case RPRM_Error_Calibration_LightLevelHigh:
        str = L"RPRM_Error_Calibration_LightLevelHigh";
        break;
    case RPRM_Error_Calibration_LightLevelLow:
        str = L"RPRM_Error_Calibration_LightLevelLow";
        break;
    case RPRM_Error_8305CameraAbsent:
        str = L"RPRM_Error_8305CameraAbsent: "
                    L"Check camera or accumulator connection";
        break;
    case RPRM_Error_NotImplemented:
        str = L"RPRM_Error_NotImplemented";
        break;
    case RPRM_Error_RemoveDocument:
        str = L"RPRM_Error_RemoveDocument";
        break;
    case RPRM_Error_BadDataFile:
        str = L"RPRM_Error_BadDataFile";
        break;
    case RPRM_Error_BadInputImage:
        str = L"RPRM_Error_BadInputImage";
        break;

    default:
        str = "Unknown";
        break;
    }
    return str;
}

//------------------------------------------------------------------------------
// Returns lighting scheme abbreviations depending on eRPRM_Lights flags
inline
UnicodeString GetLSName( const DWORD light )
{
    UnicodeString str;
    UnicodeString comma;

    auto append = [ &str, &comma ]( const UnicodeString& meaning )
    {
        str += comma + meaning;
        comma = ", ";
    };

    if( light & RPRM_Light_OVI )
    {
        append( "OVI" );
    }

    if( light & RPRM_Light_White_Top )
    {
        append( "Wt" );
    }

    if( light & RPRM_Light_White_Side )
    {
        append( "Ws" );
    }

    if( light & RPRM_Light_IR_Top )
    {
        append( "IRt" );
    }

    if( light & RPRM_Light_IR_Side )
    {
        append( "IRs" );
    }

    if( light & RPRM_Light_Transmitted )
    {
        append( "LT" );
    }

    if( light & RPRM_Light_Transmitted_IR )
    {
        append( "LT IR" );
    }

    if( light & RPRM_Light_UV )
    {
        append( "UV" );
    }

    if( light & RPRM_Light_IR_Luminescence )
    {
        append( "IRLum" );
    }

    if( light & RPRM_Light_AXIAL_White_Front )
    {
        append( "AxW+" );
    }

    if( light & RPRM_Light_AXIAL_White_Left )
    {
        append( "AxWl" );
    }

    if( light & RPRM_Light_AXIAL_White_Right )
    {
        append( "AxWr" );
    }

    if( light & RPRM_Light_IR_720 )
    {
        append( "IR720" );
    }

    if( light & RPRM_Light_IR_940 )
    {
        append( "IR940" );
    }

    if( light & RPRM_Light_Transmitted_IR940 )
    {
        append( "IR940t" );
    }

    if( light & RPRM_Light_IR_700 )
    {
        append( "IR700" );
    }

    if( light & RPRM_Light_AntiStokes )
    {
        append( "Anti" );
    }

    if( light & RPRM_Light_OVD_Left )
    {
        append( "OVDl" );
    }

    if( light & RPRM_Light_OVD_Right )
    {
        append( "OVDr" );
    }

    if( light & RPRM_Light_UVC )
    {
        append( "UVC" );
    }

    if( light & RPRM_Light_UVB )
    {
        append( "UVB" );
    }

    if( light & RPRM_Light_White_Obl )
    {
        append( "OblW" );
    }

    if( light & RPRM_Light_White_Special )
    {
        append( "Wspec" );
    }

    if( light & RPRM_Light_White_Front )
    {
        append( "W+" );
    }

    if( light & RPRM_Light_IR_Front )
    {
        append( "IR+" );
    }

    if( light & RPRM_Light_White_Gray )
    {
        append( "Wgrey" );
    }

    if( light & RPRM_Light_OVD )
    {
        append( "lOVD" );
    }

    if( light & RPRM_Light_Videodetection )
    {
        append( "Dtct" );
    }

    if( light & RPRM_Light_IR_870_Obl )
    {
        append( "IR940" );
    }

    if( light & RPRM_Light_HR_Light )
    {
        append( "HRl" );
    }

    if( light & RPRM_Light_RAW_Data )
    {
        append( "Raw" );
    }

    if( light & RPRM_Light_RAW_Data_GRBG )
    {
        append( "GRBG" );
    }

    if( light & RPRM_Light_RAW_Data_GBRG )
    {
        append( "GBRG" );
    }

    if( light & RPRM_Light_RAW_Data_RGGB )
    {
        append( "RGGB" );
    }

    if( light & RPRM_Light_RAW_Data_BGGR )
    {
        append( "BGGR" );
    }

    return str;
}

//******************************************************************************
//******************************************************************************

__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner),
          m_hLib                  {},
          m_activeDevice          {},
          m_SDKCapabilities       {},
          m_deviceConnected       {},
          m_currentVideoMode      { RPRM_VM_UNDEFINED },
          m_liStartTime           {},
          m_availableLight        {},
          Initialize              {},
          Free                    {},
          SetCallbackFunc         {},
          ExecuteCommand          {},
          CheckResult             {},
          AllocRawImageContainer  {},
          FreeRawImageContainer   {},
          CheckResultFromList     {},
          ResultTypeAvailable     {}
{
}

//------------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
    // Инициализируем указатели на изображения
    m_paintBoxImage1 = nullptr;
    m_paintBoxImage2 = nullptr;
    m_paintBoxImage3 = nullptr;
    
    // Настраиваем обработчики отрисовки
    PaintBox1->OnPaint = PaintBox1Paint;
    PaintBox2->OnPaint = PaintBox2Paint;
    PaintBox3->OnPaint = PaintBox3Paint;
    
    // Устанавливаем Anchors для правильного позиционирования
    PaintBox1->Anchors = TAnchors() << akLeft << akTop;
    PaintBox2->Anchors = TAnchors() << akLeft << akTop;
    PaintBox3->Anchors = TAnchors() << akLeft << akTop;
    Panel1->Anchors = TAnchors() << akLeft << akTop << akRight;

    // Настраиваем размеры PaintBox и Panel1
    ResizePaintBoxes();
    
    // Инициализируем ListView
    InitListView();
    
    // Инициализируем номер сканирования
    m_currentScanNumber = 0;

    m_TempImageGroup = ImageGroup();
        
    // Устанавливаем обработчик нажатия клавиш для формы
    KeyPreview = true;
    OnKeyDown = FormKeyDown;
    
    // Устанавливаем обработчик выбора элемента в ListView
    ListView1->OnSelectItem = ListView1SelectItem;
    
    // Очищаем ProgressBar
    ProgressBar->Position = 0;
    
    UpdateControls();
	TLoadLibrary();
	TIntialize();
    Connect();

    // Устанавливаем начальное значение для Label BeforeClosing
    BeforeClosing->Caption = "15";

    // Запуск отдельного потока для отслеживания времени до закрытия
    std::thread([this]() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::minutes(1));
            TThread::Synchronize(nullptr, [this]() {
                int timeLeft = StrToInt(BeforeClosing->Caption) - 1;
                if (timeLeft <= 0) {
                    Application->Terminate();
                } else {
                    BeforeClosing->Caption = IntToStr(timeLeft);
                }
            });
        }
    }).detach();
}

// Новый метод для изменения размеров PaintBox элементов
void __fastcall TForm1::ResizePaintBoxes()
{
    // Ширина в три раза меньше ширины Panel1, высота равна ширине
    int paintBoxWidth = Panel1->Width / 3;
    int paintBoxHeight = paintBoxWidth;
    
    // Устанавливаем размеры для PaintBox элементов
    PaintBox1->Width = paintBoxWidth;
    PaintBox1->Height = paintBoxHeight;
    
    PaintBox2->Width = paintBoxWidth;
    PaintBox2->Height = paintBoxHeight;
    
    PaintBox3->Width = paintBoxWidth;
    PaintBox3->Height = paintBoxHeight;
    
    // Устанавливаем позиции для каждого PaintBox
    PaintBox1->Left = 0;
    PaintBox2->Left = PaintBox1->Left + PaintBox1->Width;
    PaintBox3->Left = PaintBox2->Left + PaintBox2->Width;
    
    // Обновляем высоту Panel1 в соответствии с PaintBox
    Panel1->Height = paintBoxHeight;
    
    // Правильно устанавливаем позиции по вертикали
    PaintBox1->Top = 0;
    PaintBox2->Top = 0;
    PaintBox3->Top = 0;
    
    // Обновляем PaintBox для перерисовки
    PaintBox1->Invalidate();
    PaintBox2->Invalidate();
    PaintBox3->Invalidate();
}

// Обработка изменения размера формы
void __fastcall TForm1::FormResize(TObject *Sender)
{
    // Гарантируем, что Panel1 изменит свои размеры
    Application->ProcessMessages();
    ResizePaintBoxes();
}

//------------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject* Sender, TCloseAction& Action)
{
	TFreeLibrary();
    Action = m_hLib ? caNone : caFree;
}

//------------------------------------------------------------------------------

void __fastcall TForm1::TLoadLibrary()
{
    Memo1->Clear();

    // Get Registry sub-key holding library path depending on Windows bitness
    ::SYSTEM_INFO info {};
    ::GetSystemInfo( &info );

    const wchar_t* subKey {};
    switch( info.wProcessorArchitecture )
    {
    case PROCESSOR_ARCHITECTURE_AMD64:
        subKey = L"SOFTWARE\\Regula\\Document Reader SDK (x64)";
        break;
    case PROCESSOR_ARCHITECTURE_INTEL:
        subKey = L"SOFTWARE\\Regula\\Document Reader SDK";
        break;
    default:
        throw Exception { "Unsupported architecture" };
    }

    // Get PasspR40.dll path
    const HKEY_Helper hKey { subKey };

    // Query size of the Path
    const wchar_t* Name = L"Path";
    DWORD bufSz {};
    auto ec = ::RegQueryValueExW( hKey,
                                  Name,
                                  nullptr,
                                  nullptr,
                                  nullptr,
                                  &bufSz );
    if( ec != ERROR_SUCCESS )
    {
        throw Exception { "Cannot query Registry" };
    }

    if( 0 == bufSz )
    {
        throw Exception { "Empty Path key!" };
    }

    // Get the 'Path' key value and build a path to PasspR40.dll
    const wchar_t* libName = L"\\PasspR40.dll";
    const auto oldBufSz = bufSz;
    bufSz += std::wcslen( libName ) + sizeof( L'\0' );
    auto result = std::make_unique< wchar_t[] >( bufSz );
    std::memset( &result[0], 0, bufSz );

    ec = ::RegQueryValueExW( hKey,
                             Name,
                             nullptr,
                             nullptr,
                             reinterpret_cast< LPBYTE >( &result[ 0 ] ),
                             &bufSz );

    if( ec != ERROR_SUCCESS )
    {
        throw Exception { "Cannot read Registry" };
    }

    const auto lastChar = oldBufSz / sizeof( wchar_t ) - 1;
    if( result[ lastChar ] == L'\0' )
    {
        std::wcscpy( &result[lastChar], libName );
    }
    else
    {
        // The value returned is NOT guaranteed to be null-terminated
        std::wcscpy( &result[ lastChar + 1 ], libName );
    }

    // Load the PasspR40.dll and load its functions
    const auto& libPath = &result[ 0 ];
    m_hLib = ::LoadLibrary( libPath );
    if( !m_hLib )
    {
        System::UnicodeString msg { "Cannot load " };
        msg += libPath;
        throw Exception { msg };
    }

	 // _LibraryVersionFunc LibraryVersion {};

    try
    {
        // LoadFun( LibraryVersion, "_LibraryVersion" );
        LoadFun( Initialize, "_Initialize" );
        LoadFun( Free, "_Free" );
        LoadFun( SetCallbackFunc, "_SetCallbackFunc" );
        LoadFun( ExecuteCommand, "_ExecuteCommand" );
        LoadFun( CheckResult, "_CheckResult" );
        LoadFun( AllocRawImageContainer, "_AllocRawImageContainer" );
        LoadFun( FreeRawImageContainer, "_FreeRawImageContainer" );
        LoadFun( CheckResultFromList, "_CheckResultFromList" );
        LoadFun( ResultTypeAvailable, "_ResultTypeAvailable" );
    }
    catch( ... )
    {
        ::FreeLibrary( m_hLib );
        m_hLib = 0;
        throw;
    }

    // Set callbacks which must be called before calling the Initialize()
    SetCallbackFunc( MyResultReceivingFunc, MyNotifyFunc );

	//const auto ver = LibraryVersion();
	//LibraryVersionLabel->Caption = IntToStr( HIWORD( ver ) ) +
	//							   "." +
	//							   IntToStr( LOWORD( ver ) );

    ::QueryPerformanceCounter( &m_liStartTime );
	UpdateControls();
}

//------------------------------------------------------------------------------

void __fastcall TForm1::UpdateControls()
{
	// // Video detection button
	// SB_VideoDetection->Enabled = m_hLib;
	// SB_VideoDetection->Visible = m_hLib;

    // Connect/Disconnect button
	ReconnectDeviceButton->Enabled = m_hLib;
	ReconnectDeviceButton->Down = m_deviceConnected;

    ProcessButton->Enabled = m_deviceConnected;

    // SaveImagesCheckBox->Enabled = m_deviceConnected;
    // XMLCheckBox->Enabled = m_deviceConnected;
    // ClipboardCheckBox->Enabled = m_deviceConnected;
	// FileCheckBox->Enabled = m_deviceConnected;

    // const bool location_is_available =
    //     m_SDKCapabilities & RPRM_Capabilities_LocateDocument;

    // LocateCheckBox->Checked =
    //     LocateCheckBox->Checked ||
    //     MRZCheckBox->Checked ||
    //     MRZTestQualityCheckBox->Checked ||
    //     DocTypeCheckBox->Checked ||
    //     BarCodesCheckBox->Checked ||
    //     VisualOCRCheckBox->Checked ||
    //     AuthenticityCheckBox->Checked;

    // LocateCheckBox->Enabled =
    //     m_deviceConnected &&
    //     location_is_available &&
    //     !MRZCheckBox->Checked &&
    //     !MRZTestQualityCheckBox->Checked &&
    //     !DocTypeCheckBox->Checked &&
    //     !BarCodesCheckBox->Checked &&
    //     !VisualOCRCheckBox->Checked &&
    //     !AuthenticityCheckBox->Checked;

    // MRZCheckBox->Enabled =
    //     m_deviceConnected &&
    //     (m_SDKCapabilities & RPRM_Capabilities_MRZ_OCR) &&
    //     location_is_available;

    // ScanImagesCheckBox->Enabled =
    //     m_deviceConnected &&
    //     (m_SDKCapabilities & RPRM_Capabilities_Scan);

    // AllImagesCheckBox->Enabled =
    //     m_deviceConnected &&
    //     (m_SDKCapabilities & RPRM_Capabilities_Visual_OCR) &&
    //     location_is_available;

    // MRZTestQualityCheckBox->Enabled =
    //     m_deviceConnected &&
    //     (m_SDKCapabilities & RPRM_Capabilities_MRZ_TestQuality) &&
    //     location_is_available;

    // BarCodesCheckBox->Enabled =
    //     m_deviceConnected &&
    //     (m_SDKCapabilities & RPRM_Capabilities_BarCodes) &&
    //     location_is_available;

    // DocTypeCheckBox->Enabled =
    //     m_deviceConnected &&
    //     (m_SDKCapabilities & RPRM_Capabilities_Visual_OCR) &&
    //     location_is_available;

    // VisualOCRCheckBox->Enabled =
    //     m_deviceConnected &&
    //     (m_SDKCapabilities & RPRM_Capabilities_Visual_OCR) &&
    //     location_is_available;

    // AuthenticityCheckBox->Enabled =
    //     m_deviceConnected &&
    //     (m_SDKCapabilities & RPRM_Capabilities_Authenticity) &&
    //     location_is_available;

    // OCRLexAnalisysButton->Enabled =
	// 	m_deviceConnected &&
    //     (m_SDKCapabilities & RPRM_Capabilities_OCR_Analyze) &&
    //     location_is_available;

    {
        const auto it = std::find( m_availableLight.begin(),
                                   m_availableLight.end(),
                                   RPRM_Light_UV );
        //UVExpSpinEdit->Visible = ( it != m_availableLight.end() );
        //UVExpLabel->Visible = UVExpSpinEdit->Visible;
        UVExpSpinEdit->Enabled = m_deviceConnected;
        UVExpLabel->Enabled = UVExpSpinEdit->Enabled;
    }

    // int fl = m_activeDevice.AvailableVideoModes & RPRM_VM_1MP;
    // SB_1mp->Visible = m_deviceConnected;
    // SB_1mp->Enabled = fl;

    // fl = m_activeDevice.AvailableVideoModes & RPRM_VM_3MP;
    // SB_3mp->Visible = m_deviceConnected;
    // SB_3mp->Enabled = fl;

    // fl = m_activeDevice.AvailableVideoModes & RPRM_VM_5MP;
    // SB_5mp->Visible = m_deviceConnected;
    // SB_5mp->Enabled = fl;

    // fl = m_activeDevice.AvailableVideoModes & RPRM_VM_9MP_2;
    // SB_92mp->Visible = m_deviceConnected;
    // SB_92mp->Enabled = fl;

    // fl = m_activeDevice.AvailableVideoModes & RPRM_VM_9MP;
    // SB_9mp->Visible = m_deviceConnected;
    // SB_9mp->Enabled = fl;

    // if( m_currentVideoMode == RPRM_VM_1MP )
    // {
    //     SB_1mp->Down = true;
    // }
    // else if( m_currentVideoMode == RPRM_VM_3MP )
    // {
    //     SB_3mp->Down = true;
    // }
    // else if( m_currentVideoMode == RPRM_VM_5MP )
    // {
    //     SB_5mp->Down = true;
    // }
    // else if( m_currentVideoMode == RPRM_VM_9MP_2 )
    // {
    //     SB_92mp->Down = true;
    // }
    // else if( m_currentVideoMode == RPRM_VM_9MP )
    // {
    //     SB_9mp->Down = true;
    // }
}

//------------------------------------------------------------------------------

void __fastcall TForm1::TIntialize()
{
    const auto res = Initialize( nullptr, Handle );

    if( ( res != RPRM_Error_NoError ) &&
        ( res != RPRM_Error_AlreadyDone ) )
    {
        LogMessage( L"Initialize() return code = 0x%08X", res );
		TFreeLibrary();
    }
    else
    {
        LogMessage( L"Initialized" );
    }

    UpdateControls();
}

//------------------------------------------------------------------------------

void __fastcall TForm1::TFreeLibrary()
{
    if( ! m_hLib )
    {
        LogMessage( L"Not initialized" );
        return;
    }

    Disconnect();

    const auto res = Free();
    if( ( res == RPRM_Error_NoError ) || ( res == RPRM_Error_NotInitialized ) )
    {
        ::FreeLibrary( m_hLib );
        m_hLib = 0;

        Initialize = nullptr;
        Free = nullptr;
        SetCallbackFunc = nullptr;
        ExecuteCommand = nullptr;
        CheckResult = nullptr;
        CheckResultFromList = nullptr;
        ResultTypeAvailable = nullptr;

        //LibraryVersionLabel->Caption = "";
    }

    UpdateControls();
}

//------------------------------------------------------------------------------

void __fastcall TForm1::ReconnectDeviceButtonClick(TObject *Sender)
{
		//const auto enabled = SB_VideoDetection->Down;
		//Call< RPRM_Command_Device_UseVideoDetection >( enabled );
        TFreeLibrary();
        TLoadLibrary();
        TIntialize();
        Connect();
}

//------------------------------------------------------------------------------

void __fastcall TForm1::ProcessButtonClick(TObject *Sender)
{    
    // Восстанавливаем значение Label BeforeClosing до 3
    BeforeClosing->Caption = "15";

    // Очищаем предыдущие изображения
    if (m_paintBoxImage1) {
        m_paintBoxImage1 = nullptr;
    }
    if (m_paintBoxImage2) {
        m_paintBoxImage2 = nullptr;
    }
    if (m_paintBoxImage3) {
        m_paintBoxImage3 = nullptr;
    }
    LogMessage(L"Очищены PaintBox'ы для группы с номером: %d", m_currentScanNumber);

    // Теперь очищаем поля группы
    m_TempImageGroup.Clear();
    LogMessage(L"Очищена временная группа для сканирования с номером: %d", m_currentScanNumber);

    // Увеличиваем номер сканирования для создания новой группы
    m_currentScanNumber = ListView1->Items->Count + 1;
    m_TempImageGroup.scanNumber = m_currentScanNumber;
    LogMessage(L"Начато новое сканирование с номером: %d", m_currentScanNumber);

    // Проверяем, что у нас есть нужные значения в m_LightList
    const std::vector<DWORD> requiredLights = {6, 24, 128};
    for (const auto& light : requiredLights)
    {
        bool found = false;
        for (const auto& availableLight : m_LightList)
        {
            if (availableLight == light)
            {
                found = true;
                break;
            }
        }
        
        if (!found)
        {
            LogMessage(L"Не найдено необходимое значение освещения: %d", light);
        }
    }

    // Устанавливаем нужные значения для сканирования
    Call< RPRM_Command_Device_Light_ScanList_Clear >();
    
    // Добавляем только требуемые значения освещения (6, 24, 128)
    for (const auto& light : requiredLights)
    {
        Call< RPRM_Command_Device_Light_ScanList_AddTo >(light);
        LogMessage(L"Добавлено значение освещения для сканирования: %d", light);
    }

    // Process and get results
    //auto processingMode = GetProcessingMode();
    uint32_t processingMode = RPRM_GetImage_Modes_GetImages;

    if (ListView1->Items->Count == 1) {
        processingMode |= RPRM_GetImage_Modes_OCR_MRZ;
    }
    const auto res = Call< RPRM_Command_Process >( processingMode );
    LogMessage( L"RPRM_Command_Process return code = 0x%08X", res );
    if( res != RPRM_Error_NoError ) {
        LogMessage(L"Ошибка при сканировании");
        return;
    }
    
    /*const auto mode = GetDirectQueryMode();
    if( mode )
    {
        DoDirectQuery( mode );
    }*/

    // Логируем состояние групп перед началом сканирования
    LogMessage(L"Перед началом сканирования группы: scanNumber=%d, White=%s, IR=%s, UV=%s",
        m_TempImageGroup.scanNumber,
        m_TempImageGroup.image_light6 ? L"есть" : L"нет",
        m_TempImageGroup.image_light24 ? L"есть" : L"нет",
        m_TempImageGroup.image_light128 ? L"есть" : L"нет");
}

//------------------------------------------------------------------------------

void __fastcall TForm1::OCRLexAnalisysButtonClick(TObject *Sender)
{
    const auto res = Call< RPRM_Command_OCRLexicalAnalyze >( nullptr );
    if( res == RPRM_Error_NoError )
    {
        const auto mode = GetDirectQueryMode();
        if( mode )
        {
            GetResult< RPRM_ResultType_OCRLexicalAnalyze >(
                mode,
                [ this ]( auto ) -> UnicodeString
                {
                    const auto path = GetTestDataPath();
                    return path + "ocr_lex_an.xml";
                }
            );
        }
    }
}

//------------------------------------------------------------------------------

void __fastcall TForm1::CheckBoxClick(TObject *Sender)
{
    UpdateControls();
}

//------------------------------------------------------------------------------

void __fastcall TForm1::SB_1mpClick(TObject *Sender)
{
    SetWorkingVideoMode( RPRM_VM_1MP );
}

//------------------------------------------------------------------------------

void __fastcall TForm1::SB_3mpClick(TObject *Sender)
{
    SetWorkingVideoMode( RPRM_VM_3MP );
}

//------------------------------------------------------------------------------

void __fastcall TForm1::SB_5mpClick(TObject *Sender)
{
    SetWorkingVideoMode( RPRM_VM_5MP );
}

//------------------------------------------------------------------------------

void __fastcall TForm1::SB_92mpClick(TObject *Sender)
{
    SetWorkingVideoMode( RPRM_VM_9MP_2 );
}

//------------------------------------------------------------------------------

void __fastcall TForm1::SB_9mpClick(TObject *Sender)
{
	SetWorkingVideoMode( RPRM_VM_9MP );
}

//------------------------------------------------------------------------------

void __fastcall TForm1::UVExpSpinEditChange(TObject *Sender)
{
	SetUVExposure( UVExpSpinEdit->Value );
}
//------------------------------------------------------------------------------

void __fastcall TForm1::PaintDocPresenceBox( const TColor color )
{
    Shape1->Brush->Color = color;
}

//------------------------------------------------------------------------------

void TForm1::LogMessage( const wchar_t *fmt, ... ) const
{
    va_list args;
    va_start( args, fmt );

    UnicodeString str;
    str.vprintf( fmt, args );

    LogMessage( str );

    va_end( args );
}

//------------------------------------------------------------------------------

void TForm1::LogMessage( const UnicodeString& msg ) const
{
    LARGE_INTEGER liFinish;
    ::QueryPerformanceCounter( &liFinish );

    LARGE_INTEGER liFreq;
    ::QueryPerformanceFrequency( &liFreq );

    const double dd
    {
        static_cast< double >( liFinish.QuadPart - m_liStartTime.QuadPart )
        /
        static_cast< double >( liFreq.LowPart )
    };

    UnicodeString timestr;
    timestr.printf( TEXT("%0.4f sec"), dd );

    Memo1->Lines->Add( timestr + ":   " + msg );
}

//------------------------------------------------------------------------------

long __fastcall TForm1::GetDirectQueryMode() const
{
    long mode {};
    if( SaveImagesCheckBox->Checked )
    {
        mode |= ofrFormat_FileBuffer;
    }

    if( FileCheckBox->Checked )
    {
        mode |= ofrTransport_File;
    }

    if( XMLCheckBox->Checked )
    {
        mode |= ofrFormat_XML;
    }

    if( ClipboardCheckBox->Checked )
    {
        mode |= ofrTransport_Clipboard;
    }
    return mode;
}

//------------------------------------------------------------------------------

void __fastcall TForm1::DoDirectQuery( const long mode )
{
    DWORD imgFormat {};
    Get< RPRM_Command_Options_GraphicFormat_ActiveIndex >( &imgFormat );

    char *format {};
    Call< RPRM_Command_Options_GraphicFormat_Name >( imgFormat, &format );

    const auto path = GetTestDataPath();

    auto CopyVisualInfoToClipboard = [ this ]( const HANDLE hRes )
    {
        auto res = reinterpret_cast< const TResultContainer* >( hRes );
        auto info = static_cast< const TDocVisualExtendedInfo* >( res->buffer );
        auto fieldNum = info->nFields;
        while( fieldNum-- )
        {
            // The clipboard is being overwritten by the last iteration
            const auto rtc =
                CheckResultFromList( hRes, ofrTransport_Clipboard, 0 );
            if( rtc < 0 )
            {
                break;
            }
        }
    };

    //**************************************************************************
    // RPRM_ResultType_RawImage
    //**************************************************************************

    // Get images for all selected lightning scheme checkboxes
    if( ScanImagesCheckBox->Checked )
    {
        constexpr auto RawImage = RPRM_ResultType_RawImage;
        GetResult< RawImage >(
            mode,
            [ this, &format, &path ]( auto idx ) -> UnicodeString
            {
                // Get the lightning scheme for this id and insert
                // it into the file name
                auto hRes = CheckResult( RawImage,
                                         idx,
                                         ofrDefault,
                                         0 );

                const auto ret = reinterpret_cast< intptr_t >( hRes );
                if( ret < 0 )
                {
                    UnicodeString msg;
                    msg.printf(
                        L"CheckResult( RPRM_ResultType_RawImage, %d, "
                        L"ofrDefault, 0 ) "
                        L"failed with eRPRM_ResultStatus == %d", idx, ret );
                    throw Exception { msg };
                }

                auto res = reinterpret_cast< const TResultContainer* >( hRes );
                if( ! res )
                {
                    throw Exception
                    {
                        "RPRM_ResultType_Graphics: null TResultContainer"
                    };
                }

                UnicodeString slight;
                slight.printf( L"0x%08X", res->light );

                auto ext = XMLCheckBox->Checked ? UnicodeString( ".xml" ) :
                                                  ExtractFileExt( format );

                return path + slight + ext;
            }
        );
    }

    //**************************************************************************
    // RPRM_ResultType_MRZ_OCR_Extended
    //**************************************************************************
    if( MRZCheckBox->Checked )
    {
        GetResult< RPRM_ResultType_MRZ_OCR_Extended >(
            mode,
            [ &path ]( auto ) -> UnicodeString
            {
                return path + "mrz_parsed.xml";
            },
            CopyVisualInfoToClipboard
        );
    }

    //**************************************************************************
    // RPRM_ResultType_MRZ_TestQuality
    //**************************************************************************
    if( MRZTestQualityCheckBox->Checked )
    {
        GetResult< RPRM_ResultType_MRZ_TestQuality >(
            mode,
            [ &path ]( auto ) -> UnicodeString
            {
                return path + "mrz_test_quality.xml";
            }
        );
    }

    //**************************************************************************
    // RPRM_ResultType_BarCodes
    // RPRM_ResultType_BarCodes_TextData
    // RPRM_ResultType_BarCodes_ImageData
    //**************************************************************************
    if( BarCodesCheckBox->Checked )
    {
        GetResult< RPRM_ResultType_BarCodes >(
            mode,
            [ &path ]( auto ) -> UnicodeString
            {
                return path + "barcodes_binary.xml";
            }
        );

        GetResult< RPRM_ResultType_BarCodes_TextData >(
            mode,
            [ &path ]( auto ) -> UnicodeString
            {
                return path + "barcodes_text.xml";
            },
            CopyVisualInfoToClipboard
        );

        GetResult< RPRM_ResultType_BarCodes_ImageData >(
            mode,
            [ &path ]( auto ) -> UnicodeString
            {
                return path + "barcodes_graphics.xml";
            }
        );
    }

    //**************************************************************************
    // RPRM_ResultType_DocumentTypesCandidates
    //**************************************************************************
    if( DocTypeCheckBox->Checked )
    {
        GetResult< RPRM_ResultType_DocumentTypesCandidates >(
            mode,
            [ &path ]( auto ) -> UnicodeString
            {
                return path + "doc_type_candidates.xml";
            }
        );
    }

    //**************************************************************************
    // RPRM_ResultType_Visual_OCR_Extended
    // RPRM_ResultType_Graphics
    //**************************************************************************
    if( VisualOCRCheckBox->Checked )
    {
        GetResult< RPRM_ResultType_Visual_OCR_Extended >(
            mode,
            [ &path ]( auto ) -> UnicodeString
            {
                return path + "visual_OCR_txt.xml";
            },
            CopyVisualInfoToClipboard
        );

        GetResult< RPRM_ResultType_Graphics >(
            mode,
            [ &path ]( auto ) -> UnicodeString
            {
                return path + "visual_OCR_txt.xml";
            },
            [ this, &path, &format ]( auto hRes )
            {
                auto info =
                    reinterpret_cast< const TResultContainer* >( hRes );

                auto data =
                    static_cast< const TDocGraphicsInfo* >( info->buffer );

                auto fieldNum = data->nFields;
                while( fieldNum-- )
                {
                    UnicodeString filename =
                        path +
                       "visual_OCR_txt_graph" +
                       IntToStr( static_cast< int >( fieldNum ) ) +
                       ExtractFileExt( format );

                    const auto rtc = CheckResultFromList( hRes,
                                                          ofrTransport_File,
                                                          filename.c_str() );
                    if( rtc < 0 )
                    {
                        break;
                    }
                }
            }
        );
    }

    //**************************************************************************
    // RPRM_ResultType_Authenticity
    //**************************************************************************
    if( AuthenticityCheckBox->Checked )
    {
        GetResult< RPRM_ResultType_Authenticity >(
            mode,
            [ &path ]( auto ) -> UnicodeString
            {
                return path + "authenticity.xml";
            }
        );
    }
}

//------------------------------------------------------------------------------

void __fastcall TForm1::Connect()
{
    auto res = Call< RPRM_Command_Device_Connect >( -1 );
    m_deviceConnected =
        ( res == RPRM_Error_NoError ) || ( res == RPRM_Error_AlreadyDone );

    if( m_deviceConnected )
    {
        // Set up image containers
		//CleanupImageContainer( m_imagesTabs ); // !!!!!!!!!!!!
        m_availableLight.clear();
        m_LightList.clear(); // Очищаем массив значений освещения
        
        // Инициализируем указатели на изображения
        m_paintBoxImage1 = nullptr;
        m_paintBoxImage2 = nullptr;
        m_paintBoxImage3 = nullptr;

        // Get available device features
        long idx {}; // !!!!!!!!!!!!!!!
        Get< RPRM_Command_Device_ActiveIndex >( &idx );
        LogMessage( L"Reader Index %d", idx );

		TRegulaDeviceProperties* p_activeDevice {};
        res = Call< RPRM_Command_Device_Features >( idx, &p_activeDevice);
		const bool gotFeatures
        {
            ( res == RPRM_Error_NoError ) || ( res == RPRM_Error_AlreadyDone )
        };

		if( gotFeatures && p_activeDevice )
        {
			m_activeDevice = *p_activeDevice;
        }
        else
        {
            UnicodeString msg;
            msg.printf( L"RPRM_Command_Device_Features returned: 0x%08X", res );
            LogMessage( msg );
            throw Exception { msg };
        }

		RefreshSDKCapabilities(); // !!!!!!!!!!!!

		Get< RPRM_Command_Device_Get_WorkingVideoMode >( &m_currentVideoMode );

        // Get available light schemes
		long lightCnt {};
        Get< RPRM_Command_Device_Light_ScanList_Count >( &lightCnt );

		for( decltype( lightCnt ) i {}; i < lightCnt; ++i )
		{
			long light {};
			Call< RPRM_Command_Device_Light_ScanList_Item >( i, &light );
			const auto& abbreviation = GetLSName( light );
			
			// Добавляем только в массив m_LightList, а не в LightCheckListBox
			m_LightList.push_back(light);
			LogMessage(L"Light scheme %d: 0x%08X, %s", i, light, abbreviation.c_str());
        }

		constexpr long Recommended_UV_Value { 10 };
        SetUVExposure( Recommended_UV_Value );
    }
    else
    {
        LogMessage( L"Device_Connect return code = 0x%08X", res );
    }

    UpdateControls();
}

//------------------------------------------------------------------------------

void __fastcall TForm1::Disconnect()
{
    const auto res = Call< RPRM_Command_Device_Disconnect >();
    const bool disconnected
    {
        res == RPRM_Error_NoError ||
        res == RPRM_Error_AlreadyDone
    };

    if( disconnected )
    {
		//CleanupImageContainer( m_imagesTabs ); // ##########
		m_availableLight.clear();

        if( m_deviceConnected )
        {
            m_deviceConnected = false;
            LogMessage( L"Device disconnected" );
        }

		PaintDocPresenceBox( clGray );  // ##########
    }
    else
    {
        LogMessage( L"Device_Disconnect return code = 0x%08X", res );
    }

    UpdateControls();
}

//------------------------------------------------------------------------------

void __fastcall TForm1::SetWorkingVideoMode( const eRPRM_VideoModes mode )
{
    const auto ec = Call< RPRM_Command_Device_Set_WorkingVideoMode >( mode );
    if( ( ec == RPRM_Error_NoError ) ||
        ( ec == RPRM_Error_AlreadyDone ) )
    {
        m_currentVideoMode = mode;
    }
    else
    {
        m_currentVideoMode = RPRM_VM_UNDEFINED;
        LogMessage( L"RPRM_Command_Device_Set_WorkingVideoMode returned 0x%08X",
                    ec );
    }
}

//------------------------------------------------------------------------------

UnicodeString __fastcall TForm1::GetTestDataPath() const
{
    DWORD deviceIdx {};
    Get< RPRM_Command_Device_ActiveIndex >( &deviceIdx );

    TRegulaDeviceProperties *device {};
    Call< RPRM_Command_Device_Features >( deviceIdx, &device );

    const UnicodeString exePath = ExtractFilePath( Application->ExeName );
    return exePath +
           "TestData\\" +
           GetDeviceNameByID(device->DeviceID) +
           "\\";
}

//------------------------------------------------------------------------------

/*uint32_t __fastcall TForm1::GetProcessingMode() const
{
    uint32_t res = RPRM_GetImage_Modes_Empty;

    // Get images for all selected lightning scheme checkboxes
    if( ScanImagesCheckBox->Checked )
    {
        res |= RPRM_GetImage_Modes_GetImages;
    }

    // Crop images along document borders
    if( LocateCheckBox->Checked )
    {
        res |= RPRM_GetImage_Modes_LocateDocument;
    }

    // Read MRZ
    if( MRZCheckBox->Checked )
    {
        res |= RPRM_GetImage_Modes_OCR_MRZ;
    }

    // Return all images being taken automatically by the library
    // e.g. when reading bar codes, MRZ etc.
    if( AllImagesCheckBox->Checked )
    {
        res |= RPRM_GetImage_Modes_ReceiveAllScannedImages;
    }

    // Check MRZ
    if( MRZTestQualityCheckBox->Checked )
    {
        res |= RPRM_GetImage_Modes_OCR_TestMRZQuality;
    }

    // Read barcodes
    if( BarCodesCheckBox->Checked )
    {
        res |= RPRM_GetImage_Modes_OCR_BarCodes;
    }

    // Determine document type
    if( DocTypeCheckBox->Checked )
    {
        res |= RPRM_GetImage_Modes_DocumentType;
    }

    // OCR document filling
    if( VisualOCRCheckBox->Checked )
    {
        res |= RPRM_GetImage_Modes_OCR_Visual;
    }

    // Obtain document authenticity check results
    if( AuthenticityCheckBox->Checked )
    {
        res |= RPRM_GetImage_Modes_Authenticity;
    }

    return res;
}*/

//------------------------------------------------------------------------------

void __fastcall TForm1::RefreshSDKCapabilities()
{
    Get< RPRM_Command_Options_GetSDKCapabilities >( &m_SDKCapabilities );

    if( m_activeDevice.DeviceCtrl != RPRM_DeviceControlType_Virtual )
    {
        const bool dist =
            m_SDKCapabilities &
            RPRM_Capabilities_ImageDistortionCompensation;

        const bool refine =
            m_SDKCapabilities & RPRM_Capabilities_LocateDocument;

        LogMessage(L"Calibration status: Distortion Compensation - %s",
                   dist ? L"Ok" : L"Failed");

        LogMessage(L"Calibration status: Color/Brightness Compensation - %s",
                   refine ? L"Ok" : L"Failed");
        ProgressBar->Position = 0;
    }
}

//------------------------------------------------------------------------------

void __stdcall TForm1::MyResultReceivingFunc( TResultContainer *result,
                                              uint32_t *PostAction,
                                              uint32_t *PostActionParameter )
{
    if( !result )
    {
        return;
    }

    try {
        const TDocVisualExtendedInfo* res = nullptr; // Move initialization outside
        UnicodeString fieldName, fieldValue; // Move initialization outside
        switch( result->result_type ) {
        case RPRM_ResultType_RawImage :
        case RPRM_ResultType_RawUncroppedImage:
            Form1->ReceiveRawImage( result );
            break;

        case RPRM_ResultType_MRZ_OCR_Extended :
            res = static_cast< const TDocVisualExtendedInfo* >( result->buffer );
            if( !res ) {
                return;
            }

            Form1->LogMessage( L"Receiving MRZ OCR results..." );

            for( uint32_t i = 0; i < res->nFields; ++i ) {
                const TDocVisualExtendedField* field = &res->pArrayFields[ i ];
                if (!field || !field->FieldName || !field->Buf_Text) {
                    continue;
                }

                fieldName = field->FieldName;
                fieldValue = field->Buf_Text;

                Form1->LogMessage(L"%s:  %s", fieldName.c_str(), fieldValue.c_str());

                // Если это номер документа, сохраняем его в PassportNo
                if (fieldName == L"Document Number") {
                    if (Form1->PassportNo) {
                        Form1->PassportNo->Text = fieldValue.Trim();
                        Form1->LogMessage(L"Номер паспорта сохранен: %s", fieldValue.c_str());
                    }
                }
            }
            Form1->ReceiveRawImage( result );
            Form1->Memo1->Lines->Add( L"" );
            break;

        /*case RPRM_ResultType_FileImage:
            Form1->ReceiveFileImage( result );
            break;

        case RPRM_ResultType_Visual_OCR_Extended :
        case RPRM_ResultType_BarCodes_TextData :
            Form1->ReceiveVisualExtOCR( result );
            break;

        case RPRM_ResultType_MRZ_TestQuality :
            Form1->ReceiveMRZTestQuality( result );
            break;

        case RPRM_ResultType_Graphics :
        case RPRM_ResultType_BarCodes_ImageData:
            Form1->Receive_OCR_Graphics( result );
            break;

        case RPRM_ResultType_BarCodes:
            Form1->ReceiveBarcode( result );
            break;

        case RPRM_ResultType_DocumentTypesCandidates:
            Form1->ReceiveDocTypes( result, PostAction, PostActionParameter );
            break;

        case RPRM_ResultType_ChosenDocumentTypeCandidate:
            Form1->ReceiveDocTypeCandidate( result );
            break;

        case RPRM_ResultType_DocumentsInfoList:
            // Not used
            break;

        case RPRM_ResultType_OCRLexicalAnalyze:
            Form1->ReceiveOCRLexAnalisys( result );
            break;

        case RPRM_ResultType_Authenticity:
            Form1->ReceiveAuthenticity( result );
            break;*/
        default:
            break;
        }
    } catch (Exception& e) {
        Form1->LogMessage(L"Ошибка при обработке результата: %s", e.Message.c_str());
    } catch (...) {
        Form1->LogMessage(L"Неизвестное исключение при обработке результата");
    }
}

//------------------------------------------------------------------------------

void __stdcall TForm1::MyNotifyFunc( intptr_t rawCode, intptr_t value )
{
    const auto code = static_cast< eRPRM_NotificationCodes >( rawCode );
    switch( code )
    {
    case RPRM_Notification_Error:
    {
        if( value > 0 )
        {
            const auto& str =
                ErrorToStr( static_cast< eRPRM_ErrorCodes >( value ) );
            Form1->LogMessage( L"MyNotifyFunc() error: " + str );
        }
        break;
    }

    case RPRM_Notification_DeviceDisconnected:
    {
        Form1->Disconnect();
        break;
    }

    case RPRM_Notification_DocumentReady:
    {
        Form1->NotifyDocumentReady( value );
        break;
    }

    case RPRM_Notification_Scanning:
    {
        Form1->NotifyScanning( value );
        break;
    }

    case RPRM_Notification_Calibration:
    {
        // Not implemented
        break;
    }

    case RPRM_Notification_CalibrationProgress:
    {
        // Not implemented
        break;
    }

    case RPRM_Notification_CalibrationStep:
    {
        // Not implemented
        break;
    }

    case RPRM_Notification_EnumeratingDevices:
    {
        Form1->NotifyDevEnumeration( value );
        break;
    }

    case RPRM_Notification_ConnectingDevice:
    {
        Form1->NotifyDevConnected( value );
        break;
    }

    case RPRM_Notification_DocumentCanBeRemoved:
    {
        Form1->NotifyDocCanBeRemoved();
        break;
    }

    case RPRM_Notification_LidOpen:
    {
        Form1->NotifyLidOpen( value );
        break;
    }

    case RPRM_Notification_Processing:
    {
        Form1->NotifyProcessingDone( value );
        break;
    }

    case RPRM_Notification_DownloadingCalibrationInfo:
    {
        Form1->NotifyCalibInfoDownload( value );
        break;
    }

    case RPRM_Notification_LicenseExpired:
    {
        Form1->NotifyLicenseExpired( value );
        break;
    }

    case RPRM_Notification_OperationProgress:
    {
        Form1->NotifyProgress( value );
        break;
    }

    case RPRM_Notification_LatestAvailableSDK:
    {
        Form1->NotifySDKAvailable( value );
        break;
    }

    case RPRM_Notification_LatestAvailableDatabase:
    {
        Form1->NotifyDBAvailable( value );
        break;
    }

    case RPRM_Notification_VideoFrame:
    {
        Form1->NotifyVideoFrame(
            reinterpret_cast< TVideodetectionNotification* >( value ) );
        break;
    }

    case RPRM_Notification_CompatibilityMode:
    {
        Form1->NotifyCompatibilityMode(
            *reinterpret_cast< eDeviceLimitations* >( value ) );
        break;
    }

    default:
        Form1->LogMessage( L"MyNotifyFunc(): unexpected notification code %d",
                           code );
        break;
    }
    Form1->UpdateControls();
}

//------------------------------------------------------------------------------

void __fastcall TForm1::ReceiveRawImage(const TResultContainer* result)
{
    if (!result || !result->buffer) {
        LogMessage(L"Ошибка: получен пустой результат");
        return;
    }

    try {
        TRawImageContainer* im_res = static_cast<TRawImageContainer*>(result->buffer);
        if (!im_res) {
            LogMessage(L"Ошибка: не удалось получить изображение из результата");
            return;
        }
        LogMessage(L"Добавляем изображение в группу %d", result->light);
        AddImageToGroup(GetImage(im_res), result->light);
        result = nullptr;
    }
    catch (Exception &e) {
        LogMessage(L"Исключение при обработке изображения: %s", e.Message.c_str());
    }
    catch (...) {
        LogMessage(L"Неизвестное исключение при обработке изображения");
    }
}

//------------------------------------------------------------------------------

/*void __fastcall  TForm1::ReceiveVisualExtOCR( const TResultContainer *result )
{
    auto res = static_cast< const TDocVisualExtendedInfo* >( result->buffer );
    if( !res )
    {
        return;
    }

    UnicodeString title;
    const auto type = result->result_type;
    if( type == RPRM_ResultType_MRZ_OCR_Extended )
    {
        title = "Receiving MRZ OCR results...";
    }
    else if( type == RPRM_ResultType_Visual_OCR_Extended )
    {
        title = "Receiving Visual OCR TEXT results...";
    }
    else if( type == RPRM_ResultType_BarCodes_TextData )
    {
        title = "Receiving Barcodes TEXT results...";
    }

    LogMessage( title );

    for( uint32_t i {}; i < res->nFields; ++i )
    {
        const TDocVisualExtendedField* field = &res->pArrayFields[ i ];

        UnicodeString str;
        str += field->FieldName;
        str += ":  ";
        str += field->Buf_Text;

        const auto& result = SplitString( str, "^" );
        for( const auto& sub : result )
        {
            if( ! sub.IsEmpty() )
            {
                LogMessage( sub );
            }
        }
    }

    Memo1->Lines->Add( "" );
}

//------------------------------------------------------------------------------

void __fastcall TForm1::ReceiveFileImage( const TResultContainer *result )
{
    DWORD imgFormat {};
    Get< RPRM_Command_Options_GraphicFormat_ActiveIndex >( &imgFormat );

    char* format {};
    Call< RPRM_Command_Options_GraphicFormat_Name >( imgFormat, &format );

    const UnicodeString& ext = ExtractFileExt( format );
    LogMessage( L"Receiving file image for 0x%08X (%d byte(s) - %s)",
                result->light,
                result->buf_length,
                ext.UpperCase().c_str() + 1 );
}

//------------------------------------------------------------------------------

void __fastcall TForm1::Receive_OCR_Graphics( const TResultContainer *result )
{
    auto res = static_cast< const TDocGraphicsInfo* >( result->buffer );
    if( ! res )
    {
        return;
    }

    UnicodeString title;
    if( result->result_type == RPRM_ResultType_Graphics )
    {
        LogMessage( L"Receiving Visual OCR GRAPHIC results..." );
    }
    else if( result->result_type == RPRM_ResultType_BarCodes_TextData )
    {
        LogMessage( L"Receiving Barcodes GRAPHIC results..." );
    }

    m_imagesTabs.reserve( m_imagesTabs.size() + res->nFields );

    for( size_t i {}; i < res->nFields; i++ )
    {
        TRawImageContainer* im_res = &res->pArrayFields[ i ].image;
        if( ! im_res )
        {
            return;
        }

        const auto im = GetImage( im_res );
        if( ! im )
        {
            return;
        }

        m_imagesTabs.push_back( im );
    }

    Memo1->Lines->Add( "" );
    PaintBox1->Invalidate();
}

//------------------------------------------------------------------------------

void __fastcall TForm1::ReceiveMRZTestQuality( const TResultContainer *result )
{
    auto res = static_cast< const TDocMRZTestQuality* >( result->buffer );
    if( !res )
    {
        return;
    }

    LogMessage( "MRZ Test Quality results received." );
    Memo1->Lines->Add( "" );
}

//------------------------------------------------------------------------------

void __fastcall TForm1::ReceiveBarcode( const TResultContainer *result )
{
    auto res = static_cast< const TDocBarCodeInfo* >( result->buffer );
    if( !res )
    {
        return;
    }

    LogMessage( "Receiving Barcode binary results..." );
    Memo1->Lines->Add( "" );
}

//------------------------------------------------------------------------------

void __fastcall TForm1::ReceiveDocTypes( const TResultContainer *result,
                                         uint32_t *PostAction,
                                         uint32_t *PostActionParameter )
{
    auto res = static_cast< const TCandidatesListContainer* >( result->buffer );
    if( !res )
    {
        return;
    }

    LogMessage( "Receiving Document Type Candidates..." );

    *PostAction = RPRM_PostCalbackAction_Continue;

    LogMessage( L"Choosing 1st candidate: " +
                UnicodeString { res->Candidates[ 0 ].DocumentName } );

    Memo1->Lines->Add( "" );
}

//------------------------------------------------------------------------------

void
__fastcall TForm1::ReceiveDocTypeCandidate( const TResultContainer *result )
{
    auto res = static_cast< const TOneCandidate* >( result->buffer );
    if( !res )
    {
        return;
    }

    LogMessage( "Receiving Current Document Type Candidate info..." );
    Memo1->Lines->Add( "" );
}

//------------------------------------------------------------------------------

void __fastcall TForm1::ReceiveOCRLexAnalisys( const TResultContainer *result )
{
    auto res = static_cast< const TListVerifiedFields* >( result->buffer );
    if( !res )
    {
        return;
    }

    LogMessage( "Receiving OCR Lexical Analisys results..." );
    Memo1->Lines->Add( "" );
}

//------------------------------------------------------------------------------

void __fastcall TForm1::ReceiveAuthenticity( const TResultContainer *result )
{
    auto res = static_cast< const TAuthenticityCheckList* >( result->buffer );
    if( !res )
    {
        return;
    }

    LogMessage("Receiving Authenticity Check results...");
    Memo1->Lines->Add( "" );
}*/

//------------------------------------------------------------------------------

void __fastcall TForm1::NotifyDocumentReady( const bool isPlaced )
{
    // Добавляем задержку в 10 миллисекунд
    
    if( isPlaced )
    {
        PaintDocPresenceBox( clRed );
        LogMessage( L"Document placed" );
        
        // Проверяем, есть ли элементы в ListView, и только если их нет, то запускаем сканирование
        if (ListView1->Items->Count > 0) {
            // Если нет элементов, запускаем сканирование
            Sleep(100);
            ProcessButtonClick( nullptr );
        } else {
            LogMessage( L"Сканирование не запущено - в ListView нет элементов" );
        }
    }
    else
    {
        PaintDocPresenceBox( clLime );
        LogMessage( L"Document removed" );
    }
}

//------------------------------------------------------------------------------

void __fastcall TForm1::NotifyScanning( const bool isScanningCompleted )
{
    if( isScanningCompleted )
    {
        LogMessage( L"Scanning completed" );
    }
    else
    {
        LogMessage( L"Scanning started" );
        ::SetForegroundWindow( Handle );
    }
}

//------------------------------------------------------------------------------

void __fastcall TForm1::NotifyDevEnumeration( const bool isEnumCompleted )
{
    if( isEnumCompleted )
    {
        long total_devices {};
        Get< RPRM_Command_Device_Count >( &total_devices );
        LogMessage( L"Document readers found: %d", total_devices );
    }
    else
    {
        LogMessage( L"Detecting available document readers" );
    }
}

//------------------------------------------------------------------------------

void __fastcall TForm1::NotifyDevConnected( const bool isConnected )
{
    ReconnectDeviceButton->Enabled = isConnected;
    LogMessage( isConnected ? L"Connected" : L"Connecting. Please wait..." );
}

//------------------------------------------------------------------------------

void __fastcall TForm1::NotifyDocCanBeRemoved()
{
    Form1->LogMessage( L"The document can be removed" );
    TIndicationLED led {};
    led.wColorLed = ledGreen;
    led.wFreq = MAKEWORD( 0, 0 );
    Call< RPRM_Command_Device_LED >( &led );
}

//------------------------------------------------------------------------------

void __fastcall TForm1::NotifyLidOpen( const bool isOpen )
{
    if( isOpen )
    {
        LogMessage( L"Lid open" );
    }
    else
    {
        LogMessage( L"Lid closed" );
    }
}

//------------------------------------------------------------------------------

void __fastcall TForm1::NotifyProcessingDone( const bool isDone )
{
    if( isDone )
    {
        LogMessage( L"Processing done" );
    }
    else
    {
        LogMessage( L"Processing started" );
    }
}

//------------------------------------------------------------------------------

void __fastcall TForm1::NotifyCalibInfoDownload( const int percent )
{
    LogMessage( L"Downloading calibration info %d%%", percent );
    ProgressBar->Position = percent;
}

//------------------------------------------------------------------------------

void __fastcall TForm1::NotifyLicenseExpired( const int daysSince1900 )
{
    LogMessage( L"Number of days since January, 1, 1900 "
                L"until license was active %d",
                daysSince1900 );
}

//------------------------------------------------------------------------------

void __fastcall TForm1::NotifyProgress( const int percent )
{
    LogMessage( L"Progress: %d%%", percent );
    ProgressBar->Position = percent;
}

//------------------------------------------------------------------------------

void __fastcall TForm1::NotifySDKAvailable( const uint32_t version )
{
    LogMessage( L"The latest available SDK version: %d.%d",
                HIWORD( version ),
                LOWORD( version ) );
}

//------------------------------------------------------------------------------

void __fastcall TForm1::NotifyDBAvailable( const uint32_t version )
{
    LogMessage( L"The latest available DB version: %d.%d",
                HIWORD( version ),
                LOWORD( version ) );
}

//------------------------------------------------------------------------------

void __fastcall
TForm1::NotifyVideoFrame( const TVideodetectionNotification* frame )
{
    const auto& hdr = frame->image->bmi->bmiHeader;
    LogMessage( L"New %d bit %dx%d video frame; Sensor state 0x%08X; ",
                hdr.biBitCount,
                hdr.biWidth,
                hdr.biHeight,
                frame->sensorState );
}

//------------------------------------------------------------------------------

void __fastcall TForm1::NotifyCompatibilityMode( const eDeviceLimitations mode )
{
    if( mode == DL_NONE )
    {
        LogMessage( L"The device has no limitations" );
    }
    else
    {
        if( mode & DL_USB2 )
        {
             LogMessage( L"The device is limited by USB 2.0 port" );
        }

        if( mode & DL_32bit )
        {
             LogMessage( L"The device uses a 32-bit platform" );
        }
    }
}

//------------------------------------------------------------------------------

TRawImageContainer* __fastcall TForm1::GetImage(TRawImageContainer* im_res)
{
    if (!im_res) {
        LogMessage(L"GetImage: получен нулевой указатель на изображение");
        return nullptr;
    }
    
    auto& hdr = im_res->bmi->bmiHeader;    
    int width = hdr.biWidth;
    int height = hdr.biHeight;

    // Если изображение 8-битное, возвращаем его без изменений
    /*if (hdr.biBitCount == 8) {
        TRawImageContainer* result = nullptr;
        auto res = AllocRawImageContainer(&result, width / 2, height / 2, hdr.biBitCount, hdr.biXPelsPerMeter);

        if (res == RPRM_Error_NoError && result) {
            // Уменьшаем изображение в два раза с сохранением качества
            for (int y = 0; y < height / 2; y++) {
                uint8_t* srcRow1 = static_cast<uint8_t*>(im_res->bits) + (2 * y) * width;
                uint8_t* srcRow2 = static_cast<uint8_t*>(im_res->bits) + (2 * y + 1) * width;
                uint8_t* dstRow = static_cast<uint8_t*>(result->bits) + y * (width / 2);

                for (int x = 0; x < width / 2; x++) {
                    // Среднее значение из 4 пикселей для сохранения качества
                    dstRow[x] = (srcRow1[2 * x] + srcRow1[2 * x + 1] + srcRow2[2 * x] + srcRow2[2 * x + 1]) / 4;
                }
            }
            return result;
        } else {
            LogMessage(L"Ошибка: не удалось выделить память для уменьшенного 8-битного изображения");
            return nullptr;
        }
    }*/
    if (hdr.biBitCount == 8) {
        TRawImageContainer* result = nullptr;
        auto res = AllocRawImageContainer(&result, width, height, hdr.biBitCount, hdr.biXPelsPerMeter);

        if (res == RPRM_Error_NoError && result) {
            memcpy(result->bits, im_res->bits, hdr.biSizeImage);
            return result;
        } else {
            LogMessage(L"Ошибка: не удалось выделить память для 8-битного изображения");
            return nullptr;
        }
    }
    
    // Создаем TBitmap из im_res для 24-битных изображений
    TBitmap* bitmap = new TBitmap();
    bitmap->PixelFormat = pf24bit;
    bitmap->Width = width;
    bitmap->Height = height;

    // Копируем данные из im_res в bitmap
    int srcStride = ((hdr.biWidth * 3 + 3) & ~3); // Выравнивание по 4 байта
    for (int y = 0; y < bitmap->Height; y++) {
        uint8_t* srcRow = static_cast<uint8_t*>(im_res->bits) + (bitmap->Height - 1 - y) * srcStride;
        uint8_t* dstRow = static_cast<uint8_t*>(bitmap->ScanLine[y]);
        memcpy(dstRow, srcRow, srcStride);
    }

    // Создаем уменьшенную копию для миниатюры
    TBitmap* thumb = new TBitmap();
    thumb->PixelFormat = pf24bit;
    thumb->Width = width / 2;
    thumb->Height = height / 2;

    // Масштабируем изображение с высоким качеством
    SetStretchBltMode(thumb->Canvas->Handle, HALFTONE);
    SetBrushOrgEx(thumb->Canvas->Handle, 0, 0, NULL);
    StretchBlt(thumb->Canvas->Handle, 0, 0, thumb->Width, thumb->Height,
               bitmap->Canvas->Handle, 0, 0, bitmap->Width, bitmap->Height, SRCCOPY);

    // Выделяем память для уменьшенного изображения
    TRawImageContainer* result = nullptr;
    auto res = AllocRawImageContainer(&result, thumb->Width, thumb->Height, hdr.biBitCount, hdr.biXPelsPerMeter);

    if (res == RPRM_Error_NoError && result) {
        // Копируем данные из thumb в result
        int dstStride = ((thumb->Width * 3 + 3) & ~3);
        for (int y = 0; y < thumb->Height; y++) {
            uint8_t* srcRow = static_cast<uint8_t*>(thumb->ScanLine[y]);
            uint8_t* dstRow = static_cast<uint8_t*>(result->bits) + (thumb->Height - 1 - y) * dstStride;
            memcpy(dstRow, srcRow, dstStride);
        }

        delete bitmap;
        delete thumb;
        return result;
    } else {
        LogMessage(L"Ошибка: не удалось выделить память для уменьшенного изображения");
        delete bitmap;
        delete thumb;
        return nullptr;
    }
}/**/

//------------------------------------------------------------------------------

/*TRawImageContainer* __fastcall TForm1::GetImage(TRawImageContainer* im_res)
{
    if (!im_res) {
        LogMessage(L"GetImage: получен нулевой указатель на изображение");
        return nullptr;
    }
    
    TRawImageContainer* im {};
    auto& hdr = im_res->bmi->bmiHeader;
    
    // Логируем параметры перед вызовом
    LogMessage(L"Requesting AllocRawImageContainer with Width: %d, Height: %d, BitCount: %d", 
           hdr.biWidth, hdr.biHeight, hdr.biBitCount);

    // Пытаемся выделить память
    auto res = AllocRawImageContainer(&im, 
    hdr.biWidth, hdr.biHeight, hdr.biBitCount, hdr.biXPelsPerMeter);

    if (res == RPRM_Error_NoError && im) {
        // Успешно выделили память
        memcpy(im->bits, im_res->bits, hdr.biSizeImage);
        return im;
    } else if (res == RPRM_Error_Busy) {
        // Если система занята, возвращаем nullptr
        LogMessage(L"AllocRawImageContainer is busy");
        return nullptr;
    } else {
        // Обработка других ошибок
        LogMessage(L"AllocRawImageContainer returned 0x%08X; container is %s available", res, im ? L"" : L"not");
        return nullptr;
    }
}*/

//------------------------------------------------------------------------------

void __fastcall TForm1::SetUVExposure( long value )
{
    const auto ec = Call< RPRM_Command_Device_Set_ParamLowLight >( value );
    if( ( ec == RPRM_Error_NoError ) || ( ec == RPRM_Error_AlreadyDone ) )
    {
        LogMessage( L"UV exposure is set to %d", value );
    }
    else
    {
        LogMessage( L"RPRM_Command_Device_Set_ParamLowLight returned: "
                    L"0x%08X",
                    ec );
    }

    Get< RPRM_Command_Device_Get_ParamLowLight >( &value );
    UVExpSpinEdit->Value = value;
}

//------------------------------------------------------------------------------

void __fastcall TForm1::PaintBox1Paint(TObject *Sender)
{
	if (!m_paintBoxImage1) {
		return;
	}

	DrawImageInPaintBox(PaintBox1, m_paintBoxImage1);
}

//------------------------------------------------------------------------------

void __fastcall TForm1::PaintBox2Paint(TObject *Sender)
{
	if (!m_paintBoxImage2) {
		return;
	}

	DrawImageInPaintBox(PaintBox2, m_paintBoxImage2);
}

//------------------------------------------------------------------------------

void __fastcall TForm1::PaintBox3Paint(TObject *Sender)
{
	if (!m_paintBoxImage3) {
		return;
	}

	DrawImageInPaintBox(PaintBox3, m_paintBoxImage3);
}

//------------------------------------------------------------------------------

// Общий метод для отрисовки изображения в PaintBox
void __fastcall TForm1::DrawImageInPaintBox(TPaintBox* paintBox, TRawImageContainer* image)
{
    if (!paintBox || !image || !image->bits || !image->bmi) {
        return;
    }
    
    try {
        const auto& hdr = image->bmi->bmiHeader;
        
        // Проверка корректности размеров изображения
        if (hdr.biWidth <= 0 || hdr.biHeight <= 0 || hdr.biSizeImage <= 0) {
            LogMessage(L"Некорректные размеры изображения при отрисовке в PaintBox");
            return;
        }
        
        const float rrr = static_cast<float>(paintBox->Width) / static_cast<float>(paintBox->Height);
        const float rdd = static_cast<float>(hdr.biWidth) / static_cast<float>(hdr.biHeight);

        float ratio;
        if (rrr < rdd) {
            ratio = hdr.biWidth / static_cast<float>(paintBox->Width);
        }
        else {
            ratio = hdr.biHeight / static_cast<float>(paintBox->Height);
        }
        
        // Предотвращаем деление на ноль
        if (ratio <= 0) {
            ratio = 1.0f;
        }
        
        // Вычисляем размеры изображения для отображения
        int displayWidth = static_cast<int>(hdr.biWidth / ratio);
        int displayHeight = static_cast<int>(hdr.biHeight / ratio);
        
        // Проверяем, чтобы размеры были положительными
        if (displayWidth <= 0) displayWidth = 1;
        if (displayHeight <= 0) displayHeight = 1;
        
        // Установка высококачественного режима растяжения
        ::SetStretchBltMode(paintBox->Canvas->Handle, HALFTONE);
        ::SetBrushOrgEx(paintBox->Canvas->Handle, 0, 0, NULL);
        
        // Отрисовка изображения
        bool result = ::StretchDIBits(
            paintBox->Canvas->Handle,
            0, 0, displayWidth, displayHeight,
            0, 0, hdr.biWidth, hdr.biHeight,
            image->bits,
            image->bmi,
            DIB_RGB_COLORS,
            SRCCOPY
        );
        
        if (!result) {
            DWORD error = GetLastError();
            LogMessage(L"Ошибка при отрисовке изображения: %d", error);
        }
    }
    catch (...) {
        LogMessage(L"Неизвестное исключение при отрисовке в PaintBox");
    }
}
//---------------------------------------------------------------------------



void __fastcall TForm1::LogCheckBoxClick(TObject *Sender)
{
	if (LogCheckBox->Checked)
	{
		Memo1->Visible = true;
	} else {
		Memo1->Visible = false;
	}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToLeftButton1Click(TObject *Sender)
{
    // Поворот изображений влево (против часовой стрелки)
    if (ListView1->Selected) {
        int index = ListView1->Selected->Index; 
        RotateImagesInGroup(index, false);
        ShowImageGroup(index);
    }
    else {
        LogMessage(L"Не выбран элемент для поворота");
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToRightButton2Click(TObject *Sender)
{
    // Поворот изображений вправо (по часовой стрелке)
    if (ListView1->Selected) {
        int index = ListView1->Selected->Index; 
        RotateImagesInGroup(index, true);
        ShowImageGroup(index);
    }
    else {
        LogMessage(L"Не выбран элемент для поворота");
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DeleteButtonClick(TObject *Sender)
{
    // Получаем выбранный элемент
    if (ListView1->Selected) {
        int index = ListView1->Selected->Index; 
        DeleteImageGroup(index);

        // Выбираем предыдущий элемент после удаления, если это возможно
        if (ListView1->Items->Count > 0) {
            ListView1->Items->Item[index - 1]->Selected = true;
        } else {
            ProcessButton->Caption = L"Start";
        }
    }
    else {
        LogMessage(L"Не выбран элемент для удаления");
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CompliteButtonClick(TObject *Sender)
{
    // Проверяем, не пустое ли поле PassportNo
    if (PassportNo->Text.Trim().IsEmpty()) {
        MessageDlg(L"Предупреждение: номер паспорта не может быть пустым!", mtWarning, TMsgDlgButtons() << mbOK, 0);
        return;
    }

    if (MessageDlg(L"Вы хотите завершить работу с изображениями и сохранить их?", 
                  mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes) {
        SaveAllImages();
        ClearAll();
    }
}

//------------------------------------------------------------------------------

void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
    HandleKeyboardNavigation(Key);
}

void __fastcall TForm1::ListView1SelectItem(TObject *Sender, TListItem *Item, bool Selected)
{
    if (Selected && Item) {
        // Получаем номер сканирования из данных элемента
        int scanNumber = reinterpret_cast<int>(Item->Data);
        LogMessage(L"Выбран элемент: индекс %d, scanNumber %d", 
                   Item->Index, scanNumber);
        ShowImageGroup(Item->Index);
    }
}

void __fastcall TForm1::InitListView()
{
    // Настраиваем ListView для отображения изображений в вертикальном режиме
    ListView1->ViewStyle = vsIcon; // Используем стиль иконок
    ListView1->Align = alTop;
    ListView1->LargeImages = new TImageList(this);
    ListView1->LargeImages->Width = 100;
    ListView1->LargeImages->Height = 100;
    ListView1->HideSelection = false;
    ListView1->MultiSelect = false;
    ListView1->ShowColumnHeaders = false;
    ListView1->IconOptions->Arrangement = Vcl::Comctrls::iaTop; // Расположение элементов сверху вниз
    ListView1->IconOptions->WrapText = false; // Отключаем перенос текста
    
    ListView1->Items->Clear();
}

void __fastcall TForm1::AddImageToGroup(TRawImageContainer* image, int lightType)
{
    // Проверяем, не является ли входной параметр nullptr
    if (!image) {
        LogMessage(L"Ошибка: получено пустое изображение в AddImageToGroup");
        return;
    }

    // Логируем информацию о полученном изображении
    UnicodeString lightDesc = GetLightDescription(lightType);
    LogMessage(L"Добавление изображения: тип света %d, описание %s, scanNumber %d", 
               lightType, lightDesc.c_str(), m_currentScanNumber);
    
    // Определяем тип освещения
    bool isWhiteLight = (lightType == 6);
    bool isIRLight = (lightType == 24 || lightType == 18);
    bool isUVLight = (lightType == 128 || lightType == 80);
    
    // Проверяем освещение
    if (!isWhiteLight && !isIRLight && !isUVLight) {
        LogMessage(L"Игнорируем изображение с неподдерживаемым освещением %d", lightType);
        if (image) {
            FreeRawImageContainer(image);
            image = nullptr;
        }
        return;
    }

    // Добавляем изображение в соответствующее поле временной группы
    if (isWhiteLight) {
        if (m_TempImageGroup.image_light6) {
            LogMessage(L"Заменяем существующее White изображение во временной группе");
            FreeRawImageContainer(m_TempImageGroup.image_light6);
            m_TempImageGroup.image_light6 = nullptr;
        }
        m_TempImageGroup.image_light6 = image;
        LogMessage(L"Добавлено White изображение во временную группу, scanNumber %d", m_TempImageGroup.scanNumber);
    }
    else if (isIRLight) {
        if (m_TempImageGroup.image_light24) {
            LogMessage(L"Заменяем существующее IR изображение во временной группе");
            FreeRawImageContainer(m_TempImageGroup.image_light24);
            m_TempImageGroup.image_light24 = nullptr;
        }
        m_TempImageGroup.image_light24 = image;
        LogMessage(L"Добавлено IR изображение во временную группу, scanNumber %d", m_TempImageGroup.scanNumber);
    }
    else if (isUVLight) {
        if (m_TempImageGroup.image_light128) {
            LogMessage(L"Заменяем существующее UV изображение во временной группе");
            FreeRawImageContainer(m_TempImageGroup.image_light128); 
            m_TempImageGroup.image_light128 = nullptr;
        }
        m_TempImageGroup.image_light128 = image;
        LogMessage(L"Добавлено UV изображение во временную группу, scanNumber %d", m_TempImageGroup.scanNumber);
    }
    // Логируем текущее состояние временной группы после добавления снимка
    LogMessage(L"Состояние временной группы scanNumber=%d: White=%s, IR=%s, UV=%s",
        m_TempImageGroup.scanNumber,
        m_TempImageGroup.image_light6 ? L"есть" : L"нет",
        m_TempImageGroup.image_light24 ? L"есть" : L"нет",
        m_TempImageGroup.image_light128 ? L"есть" : L"нет");

    int index = -1;
    // Проверяем, полна ли временная группа (есть ли все три изображения)
    if (m_TempImageGroup.IsComplete()) {
        LogMessage(L"Временная группа заполнена полностью, добавляем в ListView");
        // Создаем миниатюру из White изображения для ListView
        TBitmap* thumb = CreateThumbnail(m_TempImageGroup.image_light6);
        if (thumb) {
            int imageIndex = ListView1->LargeImages->Add(thumb, nullptr);
            delete thumb;
            
            // Добавляем элемент в ListView
            TListItem* item = ListView1->Items->Add();
            item->ImageIndex = imageIndex;
            item->Caption = IntToStr(m_currentScanNumber);
            // Сохраняем указатель на ImageGroup
            item->Data = new ImageGroup(m_TempImageGroup); 
            index = item->Index;
            
            LogMessage(L"Группа с номером сканирования %d добавлена в ListView", m_TempImageGroup.scanNumber);
            Pages->Caption = IntToStr(ListView1->Items->Count);
        } else {
            LogMessage(L"Ошибка при создании миниатюры для группы");
        }
        
        // Применяем поворот в зависимости от порядкового номера в ListView
        // Порядковый номер соответствует положению элемента в ListView1
        if (m_currentScanNumber == 1) {
            // Если это первый элемент в ListView, поворачиваем влево
            LogMessage(L"Поворот первого изображения влево");
            RotateImagesInGroup(index, false); // false = поворот влево
        }
        else if (m_currentScanNumber == 3) {
            // Если это третий элемент, поворачиваем вправо
            LogMessage(L"Поворот третьего изображения вправо");
            RotateImagesInGroup(index, true); // true = поворот вправо
        }
        else if (m_currentScanNumber == 4) {
            // Если это четвертый элемент, поворачиваем влево
            LogMessage(L"Поворот четвертого изображения влево");
            RotateImagesInGroup(index, false); // false = поворот влево
        }
        else if (m_currentScanNumber == 5) {
            // Если это пятый элемент, поворачиваем вправо
            LogMessage(L"Поворот пятого изображения вправо");
            RotateImagesInGroup(index, true); // true = поворот вправо
        }
    
        if (index >= 0) {
            LogMessage(L"Имитируем выбор последнего добавленного элемента в ListView1");    
            ListView1->Items->Item[index]->Selected = true;
            ProcessButton->Caption = L"Scan Next Page";
        }
    } else {
        LogMessage(L"Временная группа еще не заполнена полностью");
    }
}

void __fastcall TForm1::ShowImageGroup(int index)
{
    if (index < 0 || index >= ListView1->Items->Count) {
        LogMessage(L"Неверный индекс группы изображений: %d", index);
        return;
    }
    ListView1->Items->Item[index]->Focused = true;
    ListView1->Items->Item[index]->MakeVisible(false);
    
    TListItem* item = ListView1->Items->Item[index];
    ImageGroup* group = static_cast<ImageGroup*>(item->Data);
    
    // Логируем информацию о выбранной группе
    LogMessage(L"Показываем группу изображений: индекс %d, номер сканирования %d", 
               index, group->scanNumber);
    
    // Освобождаем предыдущие изображения в PaintBox
    if (m_paintBoxImage1) {
        m_paintBoxImage1 = nullptr;
    }
    if (m_paintBoxImage2) {
        m_paintBoxImage2 = nullptr;
    }
    if (m_paintBoxImage3) {
        m_paintBoxImage3 = nullptr;
    }
    
    // Копируем изображения для отображения
    if (group->image_light6) {
        m_paintBoxImage1 = group->image_light6;
        LogMessage(L"Отображено изображение White в PaintBox1");
    }
    if (group->image_light24) {
        m_paintBoxImage2 = group->image_light24;
        LogMessage(L"Отображено изображение IR в PaintBox2");
    }
    if (group->image_light128) {
        m_paintBoxImage3 = group->image_light128;
        LogMessage(L"Отображено изображение UV в PaintBox3");
    }
    
    // Обновляем PaintBox
    PaintBox1->Invalidate();
    PaintBox2->Invalidate();
    PaintBox3->Invalidate();
    
    LogMessage(L"Отображена группа изображений с индексом %d, scanNumber %d", 
               index, group->scanNumber);
}

void __fastcall TForm1::DeleteImageGroup(int index)
{
    if (index < 0 || index >= ListView1->Items->Count) {
        LogMessage(L"Неверный индекс группы изображений для удаления: %d", index);
        return;
    }

    if (ListView1->Items->Count == 1) {
        ClearAll();
        return;
    }
    
    LogMessage(L"Удаление группы изображений: индекс %d", index);
    std::map<int, ImageGroup*> tempImageGroups;
    for (int i = 0; i < ListView1->Items->Count; i++) {
        TListItem* item = ListView1->Items->Item[i];
        ImageGroup* group = static_cast<ImageGroup*>(item->Data);
    
        if (i < index) {
            tempImageGroups[i] = group;
            LogMessage(L"Добавлена группа: индекс %d номер сканирования %d",
             i, group->scanNumber);
        } else if (i == index) {
            FreeRawImageContainer(group->image_light24);
            group->image_light24 = nullptr;
            delete group;
            group = nullptr;

        } else if (i > index) {
            group->scanNumber = i;
            tempImageGroups[i-1] = group;
            LogMessage(L"Добавлена группа: индекс %d номер сканирования %d",
             i-1, group->scanNumber);
        }
    }

    ListView1->Items->Clear();
    ListView1->LargeImages->Clear();

    for(auto &pair : tempImageGroups)
    {
        ImageGroup* group = pair.second;

        TBitmap* thumb = nullptr;
        if(group->image_light6)
        {
            thumb = CreateThumbnail(group->image_light6);
        }
        else
        {
            // создаём белую заглушку
            thumb = new TBitmap();
            thumb->PixelFormat = pf24bit;
            thumb->Width = 100;
            thumb->Height = 100;
            thumb->Canvas->Brush->Color = clWhite;
            thumb->Canvas->FillRect(TRect(0,0,100,100));
        }

        int imgIdx = ListView1->LargeImages->Add(thumb,nullptr);
        delete thumb;

        TListItem* item = ListView1->Items->Add();
        item->ImageIndex = imgIdx;
        item->Caption = IntToStr(group->scanNumber);
        item->Data = reinterpret_cast<void*>(group); // Index
    }

    if (ListView1->Items->Item[index-1]) {
        ListView1->Items->Item[index-1]->Selected = true;
    } else {
        ListView1->Items->Item[index]->Selected = true;
    }
    Pages->Caption = IntToStr(ListView1->Items->Count);    
    LogMessage(L"Удалена группа изображений с индексом %d", index);
}

void __fastcall TForm1::RotateImagesInGroup(int index, bool clockwise)
{
    if (index < 0 || index >= ListView1->Items->Count) {
        LogMessage(L"Неверный индекс группы изображений для поворота: %d", index);
        return;
    }
    
    ImageGroup* group =  static_cast<ImageGroup*>(ListView1->Items->Item[index]->Data);
    
    // Поворачиваем каждое изображение в группе
	if (group->image_light6) {
        TRawImageContainer* rotated = RotateImage(group->image_light6, clockwise);
        if (rotated) {
			FreeRawImageContainer(group->image_light6);
            group->image_light6 = nullptr;
			group->image_light6 = rotated;
        }
    }    
	if (group->image_light24) {
		TRawImageContainer* rotated = RotateImage(group->image_light24, clockwise);
        if (rotated) {
			FreeRawImageContainer(group->image_light24);
            group->image_light24 = nullptr;
			group->image_light24 = rotated;
        }
    }    
	if (group->image_light128) {
		TRawImageContainer* rotated = RotateImage(group->image_light128, clockwise);
        if (rotated) {
			FreeRawImageContainer(group->image_light128);
            group->image_light128 = nullptr;
			group->image_light128 = rotated;
        }
    }
    
    // Обновляем миниатюру в ListView
	if (group->image_light6) {
        // Находим элемент в ListView по scanNumber
        for (int i = 0; i < ListView1->Items->Count; i++) {
            TListItem* item = ListView1->Items->Item[i];
            int itemScanNumber = reinterpret_cast<int>(item->Data);
            
            if (reinterpret_cast<int>(item->Index) == index) {
                // Создаем новую миниатюру
                TBitmap* thumb = CreateThumbnail(group->image_light6);
                if (thumb) {
                    // Заменяем старую миниатюру в ImageList
                    ListView1->LargeImages->Replace(item->ImageIndex, thumb, nullptr);
                    delete thumb;
                    LogMessage(L"Обновлена миниатюра в ListView для группы %d, scanNumber %d", 
							  index, group->scanNumber);
                }
                break;
            }
        }
    }

    // Обновляем размеры после поворота
    ResizePaintBoxes();
    
    LogMessage(L"Повернуты изображения в группе %d %s", index, clockwise ? L"по часовой стрелке" : L"против часовой стрелки");
}

TRawImageContainer* __fastcall TForm1::RotateImage(TRawImageContainer* image, bool clockwise)
{
    if (!image) {
        return nullptr;
    }
    
    // Получаем размеры и формат изображения
    const auto& hdr = image->bmi->bmiHeader;
    int width = hdr.biWidth;
    int height = hdr.biHeight;
    int bpp = hdr.biBitCount;
    
    // Создаем новый контейнер для повернутого изображения (ширина и высота меняются местами)
    TRawImageContainer* rotated = nullptr;
    auto res = AllocRawImageContainer(&rotated, height, width, bpp, hdr.biXPelsPerMeter);
    
    if (res != RPRM_Error_NoError || !rotated) {
        LogMessage(L"Не удалось создать контейнер для повернутого изображения");
        return nullptr;
    }
    
    // Копируем заголовок
    rotated->bmi->bmiHeader = hdr;
    rotated->bmi->bmiHeader.biWidth = height;  // Меняем ширину и высоту местами
    rotated->bmi->bmiHeader.biHeight = width;
    
    // Для 8-битных изображений копируем палитру
    if (bpp == 8) {
        memcpy(rotated->bmi->bmiColors, image->bmi->bmiColors, 256 * sizeof(RGBQUAD));
    }
    
    // Поворачиваем изображение с учетом текущей ориентации
    int bytesPerPixel = bpp / 8;
    if (bytesPerPixel < 1) bytesPerPixel = 1;
    
    uint8_t* srcBits = static_cast<uint8_t*>(image->bits);
    uint8_t* dstBits = static_cast<uint8_t*>(rotated->bits);
    
    int srcStride = ((width * bytesPerPixel + 3) & ~3);
    int dstStride = ((height * bytesPerPixel + 3) & ~3);
    
    // Корректируем поворот с учетом ориентации
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int srcOffset = y * srcStride + x * bytesPerPixel;
            
            // Вычисляем позицию в повернутом изображении с учетом ориентации
            int dstX, dstY;
            if (clockwise) {
                dstX = y;
                dstY = width - 1 - x; // Инвертировано для согласования с PaintBox
            } else {
                dstX = height - 1 - y; // Инвертировано для согласования с PaintBox
                dstY = x;
            }
            
            int dstOffset = dstY * dstStride + dstX * bytesPerPixel;
            
            // Копируем пиксель
            for (int b = 0; b < bytesPerPixel; b++) {
                dstBits[dstOffset + b] = srcBits[srcOffset + b];
            }
        }
    }
    
    // Обновляем размер изображения в заголовке
    rotated->bmi->bmiHeader.biSizeImage = dstStride * width;
    
    return rotated;
}

void __fastcall TForm1::HandleKeyboardNavigation(WORD key)
{
    switch (key) {      
        case VK_LEFT: {
            // Поворот изображений влево
            if (ListView1->Selected) {
                int index = ListView1->Selected->Index; 
                RotateImagesInGroup(index, false);
                ShowImageGroup(index);
            }
            break;
        }
        
        case VK_RIGHT: {
            // Поворот изображений вправо
            if (ListView1->Selected) {
                int index = ListView1->Selected->Index; 
                RotateImagesInGroup(index, true);
                ShowImageGroup(index);
            }
            break;
        }
        
        case VK_RETURN: {
            // Завершение работы с изображениями (Enter = CompliteButton)
            CompliteButtonClick(nullptr);
            break;
        }
    }
}

void __fastcall TForm1::SaveAllImages()
{
    // Запоминаем время начала выполнения
    LARGE_INTEGER startTime, endTime, frequency;
    QueryPerformanceCounter(&startTime);
    QueryPerformanceFrequency(&frequency);
    
    // Создаем базовую директорию Results, если её нет
    UnicodeString resultsDir = ExtractFilePath(Application->ExeName) + "Results";
    if (!DirectoryExists(resultsDir)) {
        if (!CreateDir(resultsDir)) {
            LogMessage(L"Не удалось создать директорию Results");
            return;
        }
    }

    // Создаем временную директорию с именем паспорта
    UnicodeString passportDir = resultsDir + "\\" + PassportNo->Text.Trim();
    if (!DirectoryExists(passportDir)) {
        if (!CreateDir(passportDir)) {
            LogMessage(L"Не удалось создать директорию для паспорта");
            return;
        }
    }

    // Инициализируем прогресс-бар
    ProgressBar->Position = 0;
    // Подсчитываем общее количество файлов для сохранения (3 файла на группу: White, IR, UV)
    int totalFileCount = ListView1->Items->Count * 3;
    int currentFileCount = 0;

    // Сохраняем все изображения
    for (int i = 0; i < ListView1->Items->Count; i++) {
        ImageGroup* group = static_cast<ImageGroup*>(ListView1->Items->Item[i]->Data);
        
        if (group->image_light6) {
            UnicodeString filename = passportDir + "\\" + PassportNo->Text.Trim() + "_Normal_" + IntToStr(static_cast<int>(i + 1)) + ".jpeg";
            SaveImageToPNG(group->image_light6, filename);
            // Обновляем прогресс
            currentFileCount++;
            ProgressBar->Position = (currentFileCount * 100) / totalFileCount;
            Application->ProcessMessages(); // Позволяем обновить GUI
        }
        
        if (group->image_light24) {
            UnicodeString filename = passportDir + "\\" + PassportNo->Text.Trim() + "_IR_" + IntToStr(static_cast<int>(i + 1)) + ".jpeg";
            SaveImageToPNG(group->image_light24, filename);
            // Обновляем прогресс
            currentFileCount++;
            ProgressBar->Position = (currentFileCount * 100) / totalFileCount;
            Application->ProcessMessages();
        }
        
        if (group->image_light128) {
            UnicodeString filename = passportDir + "\\" + PassportNo->Text.Trim() + "_UV_" + IntToStr(static_cast<int>(i + 1)) + ".jpeg";
            SaveImageToPNG(group->image_light128, filename);
            // Обновляем прогресс
            currentFileCount++;
            ProgressBar->Position = (currentFileCount * 100) / totalFileCount;
            Application->ProcessMessages();
        }
    }

    // Создаем архив с использованием TZipFile
    UnicodeString zipName = PassportNo->Text.Trim() + ".zip";
    UnicodeString zipPath = resultsDir + "\\" + zipName;

    try {
        // Создаем список файлов для архивации
        TStringList* fileList = new TStringList();
        try {
            // Получаем список файлов в директории
            TSearchRec sr;
            UnicodeString searchMask = passportDir + "\\*.*";
            
            if (FindFirst(searchMask, faAnyFile, sr) == 0) {
                do {
                    if ((sr.Attr & faDirectory) != faDirectory) {
                        fileList->Add(passportDir + "\\" + sr.Name); // Добавляем полный путь к файлу
                    }
                } while (FindNext(sr) == 0);
                
                FindClose(sr);
            }
            
            // Создаем ZIP архив
            if (fileList->Count > 0) {
                TZipFile* zipFile = new TZipFile();
                try {
                    zipFile->Open(zipPath, zmWrite);
                    
                    // Добавляем файлы в архив и обновляем прогресс
                    int zipTotalFiles = fileList->Count;
                    for (int i = 0; i < zipTotalFiles; i++) {
                        UnicodeString fileName = fileList->Strings[i];
                        UnicodeString entryName = ExtractFileName(fileName); // Используем только имя файла
                        zipFile->Add(fileName, entryName);
                        
                        // Обновляем прогресс (от 0 до 100)
                        ProgressBar->Position = ((i + 1) * 100) / zipTotalFiles;
                        Application->ProcessMessages();
                    }
                    
                    zipFile->Close();
                    LogMessage(L"Архив успешно создан: %s", zipPath.c_str());
                }
                __finally {
                    delete zipFile;
                }
            }
        }
        __finally {
            delete fileList;
        }
        
        // Удаляем временную директорию и все её содержимое
        try {
            TDirectory::Delete(passportDir, true);
            LogMessage(L"Временная директория удалена: %s", passportDir.c_str());
        }
        catch (Exception& e) {
            LogMessage(L"Ошибка при удалении временной директории: %s", e.Message.c_str());
        }
    }
    catch (Exception& e) {
        LogMessage(L"Ошибка при создании ZIP-архива: %s", e.Message.c_str());
    }
    
    // Вычисляем время выполнения
    QueryPerformanceCounter(&endTime);
    double elapsedTime = static_cast<double>(endTime.QuadPart - startTime.QuadPart) / 
                        static_cast<double>(frequency.QuadPart);
    
    // Выводим информацию о времени выполнения
    LogMessage(L"Архивирование завершено. Время выполнения: %.2f секунд", elapsedTime);
    
    // Очищаем ProgressBar по окончанию работы
    ProgressBar->Position = 0;
}

// ... existing code ...

void __fastcall TForm1::SaveImageToPNG(TRawImageContainer* image, const UnicodeString& filename)
{
    if (!image || !image->bits || !image->bmi) {
        LogMessage(L"Ошибка: пустое изображение при сохранении %s", filename.c_str());
        return;
    }

	TBitmap* bitmap = new TBitmap();
	TJPEGImage* jpg = new TJPEGImage();
	try {
        try {
            const auto& hdr = image->bmi->bmiHeader;
            
            // Устанавливаем размеры и пиксельный формат в зависимости от битности изображения
            if (hdr.biBitCount == 8) {
                // Для 8-битного изображения (IR) конвертируем в 24-бит
                bitmap->PixelFormat = pf24bit;
                bitmap->Width = hdr.biWidth;
                bitmap->Height = hdr.biHeight;
                
                // Копируем и конвертируем данные из 8-бит в 24-бит
                int srcStride = ((hdr.biWidth + 3) & ~3);
                
                for (int y = 0; y < bitmap->Height; y++) {
                    uint8_t* srcRow = (uint8_t*)image->bits + (bitmap->Height - 1 - y) * srcStride;
                    uint8_t* dstRow = (uint8_t*)bitmap->ScanLine[y];
                    
                    for (int x = 0; x < hdr.biWidth; x++) {
                        uint8_t grayValue = srcRow[x];
                        // Конвертируем оттенки серого в RGB
                        dstRow[x * 3 + 0] = grayValue; // B
                        dstRow[x * 3 + 1] = grayValue; // G
                        dstRow[x * 3 + 2] = grayValue; // R
                    }
                }
            } else {
                // Для 24-битных изображений (White, UV)
                bitmap->PixelFormat = pf24bit;
                bitmap->Width = hdr.biWidth;
                bitmap->Height = hdr.biHeight;
                
                int stride = ((hdr.biWidth * 3 + 3) & ~3);
                
                for (int y = 0; y < bitmap->Height; y++) {
                    uint8_t* srcRow = (uint8_t*)image->bits + (bitmap->Height - 1 - y) * stride;
                    uint8_t* dstRow = (uint8_t*)bitmap->ScanLine[y];
                    memcpy(dstRow, srcRow, hdr.biWidth * 3);
                }
            }
            
			// Создаем JPEG изображение
            try {
                // Устанавливаем качество JPEG (90%)
                jpg->CompressionQuality = 90;
                jpg->Assign(bitmap);
                
                // Меняем расширение файла на .jpg
                UnicodeString jpgFilename = ChangeFileExt(filename, ".jpg");
                
                // Создаем директорию если нужно
                UnicodeString dir = ExtractFileDir(jpgFilename);
                if (!DirectoryExists(dir)) {
                    if (!ForceDirectories(dir)) {
                        LogMessage(L"Не удалось создать директорию: %s", dir.c_str());
                        return;
                    }
                }
                
                // Сохраняем JPEG
                jpg->SaveToFile(jpgFilename);
                LogMessage(L"Изображение успешно сохранено как JPEG: %s", jpgFilename.c_str());
            }
            __finally {
                delete jpg;
            }
        }
        __finally {
            delete bitmap;
        }
    }
    catch (Exception& e) {
        delete jpg;
        delete bitmap;
        LogMessage(L"Ошибка при сохранении изображения %s: %s", filename.c_str(), e.Message.c_str());
    }
    catch (...) {
        delete jpg;
        delete bitmap;
        LogMessage(L"Неизвестное исключение при сохранении %s", filename.c_str());
    }
}

// Добавляем вспомогательный метод для преобразования числового значения освещения в строковое
UnicodeString __fastcall TForm1::GetLightDescription(DWORD light)
{
    // Используем функцию GetLSName для получения строкового представления
    UnicodeString lsName = GetLSName(light);
    
    // Исходя из требований, соотносим значения 6, 24, 128 с Wt, IRt, UV
    if (light == 6) {
        return L"White (Wt)";
    }
    else if (light == 24 || light == 18) {
        return L"IR (IRt)";
    }
    else if (light == 128 || light == 80) {
        return L"UV";
    }
    
    // Если не совпадает с известными значениями, возвращаем полное имя
    return lsName;
}

TBitmap* __fastcall TForm1::CreateThumbnail(TRawImageContainer* image)
{
    if (!image) {
        return nullptr;
    }
    int width = 100; 
    int height = 100;

    TBitmap* thumb = new TBitmap();
    TBitmap* bitmap = new TBitmap();
    TJPEGImage* jpegImage = new TJPEGImage();

    try {
        const auto& hdr = image->bmi->bmiHeader;
        bitmap->PixelFormat = pf24bit;
        bitmap->Width = hdr.biWidth;
        bitmap->Height = hdr.biHeight;
        
        // Копируем данные изображения с коррекцией ориентации
        int srcStride = ((hdr.biWidth * 3 + 3) & ~3); // Выравнивание по 4 байта
        
        for (int y = 0; y < bitmap->Height; y++) {
            // Переворачиваем строки по вертикали для правильной ориентации
            uint8_t* srcRow = static_cast<uint8_t*>(image->bits) + (bitmap->Height - 1 - y) * srcStride;
            uint8_t* dstRow = static_cast<uint8_t*>(bitmap->ScanLine[y]);
            
            for (int x = 0; x < bitmap->Width; x++) {
                // Копируем BGR (DIB хранит в BGR формате)
                dstRow[x * 3 + 0] = srcRow[x * 3 + 0]; // B
                dstRow[x * 3 + 1] = srcRow[x * 3 + 1]; // G
                dstRow[x * 3 + 2] = srcRow[x * 3 + 2]; // R
            }
        }
        
        // Создаем уменьшенную копию для миниатюры
        thumb->PixelFormat = pf24bit;
        thumb->Width = width;
        thumb->Height = height;
        
        // Масштабируем изображение с высоким качеством
        SetStretchBltMode(thumb->Canvas->Handle, HALFTONE);
        SetBrushOrgEx(thumb->Canvas->Handle, 0, 0, NULL);
        StretchBlt(
            thumb->Canvas->Handle, 0, 0, width, height,
            bitmap->Canvas->Handle, 0, 0, bitmap->Width, bitmap->Height,
            SRCCOPY
        );

        // Создаем JPEG изображение для уменьшения качества
        jpegImage->Assign(thumb);
        jpegImage->CompressionQuality = 50; // Устанавливаем качество JPEG на 50%

        // Копируем данные из TRawImageContainer в TBitmap
        StretchBlt(thumb->Canvas->Handle, 0, 0, width, height, 
        bitmap->Canvas->Handle, 0, 0, bitmap->Width, bitmap->Height, SRCCOPY);

        // Преобразуем обратно в TBitmap
        thumb->Assign(jpegImage);

        delete jpegImage;
        delete bitmap;
        return thumb;
    }
    catch (...) {
        delete jpegImage;
        delete bitmap;
        delete thumb;
        return nullptr;
    }
}

void __fastcall TForm1::ClearClick(TObject *Sender)
{
	ClearAll();
}

void __fastcall TForm1::ClearAll()
{
    CleanupListView();
    
    // Обновляем PaintBox
    PaintBox1->Invalidate();
    PaintBox2->Invalidate();
    PaintBox3->Invalidate();
    
    m_currentScanNumber = ListView1->Items->Count;
    Pages->Caption = IntToStr(m_currentScanNumber);
    PassportNo->Text = "";
    ProcessButton->Caption = L"Start";
    
    LogMessage(L"Все изображения очищены, счетчик сканирований сброшен");
}

// Добавляем реализацию функции CleanupImageContainer
void __fastcall TForm1::CleanupListView()
{
    // Освобождаем все изображения
    for (int i = 0; i < ListView1->Items->Count; ++i) {
        TListItem* item = ListView1->Items->Item[i];
        ImageGroup* group = static_cast<ImageGroup*>(item->Data);
        if (group) {
            if (group->image_light6) {
                FreeRawImageContainer(group->image_light6);
                group->image_light6 = nullptr;
            }
            if (group->image_light24) {
                FreeRawImageContainer(group->image_light24);
                group->image_light24 = nullptr;
            }
            if (group->image_light128) {
                FreeRawImageContainer(group->image_light128);
                group->image_light128 = nullptr;
            }
            delete group;
            group = nullptr;
        }
    }
    
    // Очищаем ListView
    ListView1->Items->Clear();
    ListView1->LargeImages->Clear();
    
    // Очищаем PaintBox
    if (m_paintBoxImage1) {
        m_paintBoxImage1 = nullptr;
    }
    if (m_paintBoxImage2) {
        m_paintBoxImage2 = nullptr;
    }
    if (m_paintBoxImage3) {
        m_paintBoxImage3 = nullptr;
    }
}


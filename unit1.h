﻿//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.CheckLst.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

#include "PasspR.h"
#include "cspin.h"
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>

#include <vector>
#include <map>

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:    // IDE-managed Components    TLabel *LibraryVersionLabel;
    TPanel *FDSPanel;
    TBevel *Bevel1;
    TMemo *Memo1;
	TSpeedButton *ReconnectDeviceButton;
    TShape *Shape1;
    TCheckBox *SaveImagesCheckBox;
    TCheckBox *LocateCheckBox;
    TCheckBox *XMLCheckBox;
    TCheckBox *ClipboardCheckBox;
    TCheckBox *FileCheckBox;
    TCheckBox *MRZCheckBox;
    TCheckBox *MRZTestQualityCheckBox;
    TCheckBox *ScanImagesCheckBox;
    TCheckBox *BarCodesCheckBox;
    TCheckBox *DocTypeCheckBox;
    TCheckBox *VisualOCRCheckBox;
    TCheckBox *AuthenticityCheckBox;
    TButton *OCRLexAnalisysButton;
    TProgressBar *ProgressBar;
    TCheckBox *AllImagesCheckBox;
    TCSpinEdit *UVExpSpinEdit;
    TSpeedButton *SB_VideoDetection;
    TLabel *UVExpLabel;
	TPaintBox *PaintBox1;
	TPaintBox *PaintBox2;
	TPaintBox *PaintBox3;
	TPanel *Panel1;
	TCheckBox *LogCheckBox;
	TPanel *Panel2;
	TListView *ListView1;
	TEdit *PassportNo;
	TLabel *Pages;
	TLabel *Label2;
	TPanel *Panel3;
	TButton *Clear;
	TButton *ToLeftButton1;
	TButton *DeleteButton;
	TButton *ToRightButton2;
	TButton *CompliteButton;
	TButton *ProcessButton;
	TLabel *Label1;
	TLabel *BeforeClosing;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormCreate(TObject *Sender);
	void __fastcall ReconnectDeviceButtonClick(TObject *Sender);
    void __fastcall ProcessButtonClick(TObject *Sender);
	void __fastcall CheckBoxClick(TObject *Sender);
    void __fastcall OCRLexAnalisysButtonClick(TObject *Sender);
    void __fastcall SB_1mpClick(TObject *Sender);
    void __fastcall SB_3mpClick(TObject *Sender);
    void __fastcall SB_5mpClick(TObject *Sender);
    void __fastcall SB_92mpClick(TObject *Sender);
	void __fastcall SB_9mpClick(TObject *Sender);
	void __fastcall UVExpSpinEditChange(TObject *Sender);
	void __fastcall PaintBox1Paint(TObject *Sender);
	void __fastcall PaintBox2Paint(TObject *Sender);
	void __fastcall PaintBox3Paint(TObject *Sender);
	void __fastcall LogCheckBoxClick(TObject *Sender);
	void __fastcall ToLeftButton1Click(TObject *Sender);
	void __fastcall ToRightButton2Click(TObject *Sender);
	void __fastcall DeleteButtonClick(TObject *Sender);
	void __fastcall CompliteButtonClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall ListView1SelectItem(TObject *Sender, TListItem *Item, bool Selected);
	void __fastcall ClearClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);

private:
    // Новый метод для изменения размеров PaintBox элементов
    void __fastcall ResizePaintBoxes();

    // Структура для хранения группы изображений с одинаковым порядковым номером
    struct ImageGroup {
        TRawImageContainer* image_light6;   // Освещение 6 - соответствует PaintBox1
        TRawImageContainer* image_light24;  // Освещение 24 - соответствует PaintBox2
        TRawImageContainer* image_light128; // Освещение 128 - соответствует PaintBox3
        int scanNumber;                     // Порядковый номер сканирования
        
        ImageGroup() : image_light6(nullptr), image_light24(nullptr), image_light128(nullptr), scanNumber(0) {}
        
        // Метод для проверки полноты группы
        bool IsComplete() const {
            return image_light6 != nullptr && image_light24 != nullptr && image_light128 != nullptr;
        }
        
        // Метод для очистки группы
        void Clear() {
            image_light6 = nullptr;
            image_light24 = nullptr;
            image_light128 = nullptr;
        }
    };

    // Library handle
    HMODULE m_hLib;

    // Document reader properties
    TRegulaDeviceProperties m_activeDevice;

    // Functional capabilities
    DWORD m_SDKCapabilities;

    // Indicates whether the device is connected
    bool m_deviceConnected;

    // Current video mode
    eRPRM_VideoModes m_currentVideoMode;

    // The last timestamp for measuring time between two
    // consecutive log messages
    LARGE_INTEGER m_liStartTime;

    // Received scanned images
    //std::vector< TRawImageContainer* > m_imagesTabs;

    // Available light schemes
    std::vector< DWORD > m_availableLight;
    
    // Необходимые значения освещения для PaintBox1, PaintBox2, PaintBox3
    std::vector< DWORD > m_LightList;

    // Указатели на изображения для отображения в PaintBox1, PaintBox2, PaintBox3
    TRawImageContainer* m_paintBoxImage1;
    TRawImageContainer* m_paintBoxImage2;
    TRawImageContainer* m_paintBoxImage3;
        
    // Временная коллекция для незавершенных групп изображений
    ImageGroup m_TempImageGroup;
    
    // Номер следующего сканирования
    int m_currentScanNumber;

    // Вспомогательная функция для создания копии TRawImageContainer с новым выделением памяти
    TRawImageContainer* __fastcall GetImage(TRawImageContainer* im_res);

    // Вспомогательный метод для освобождения памяти контейнера изображений
    void __fastcall CleanupImageContainer(std::vector<TRawImageContainer*>& container);

    // Вспомогательный метод для сохранения изображения в файл PNG
    void __fastcall SaveImageToPNG(TRawImageContainer* image, const UnicodeString& filename);
    
    // Вспомогательный метод для отрисовки изображения в PaintBox
    void __fastcall DrawImageInPaintBox(TPaintBox* paintBox, TRawImageContainer* image);
    
    // Вспомогательный метод для поворота изображения
    TRawImageContainer* __fastcall RotateImage(TRawImageContainer* image, bool clockwise);

    // Новый метод для выполнения процессинга
    void __fastcall ExecuteProcessing();

    // Imported library functions
    _InitializeFunc Initialize;
    _FreeFunc Free;
    _SetCallbackFuncFunc SetCallbackFunc;
    _ExecuteCommandFunc ExecuteCommand;
    _CheckResultFunc CheckResult;
    _CheckResultFromListFunc CheckResultFromList;
    _ResultTypeAvailableFunc ResultTypeAvailable;
    _AllocRawImageContainerFunc AllocRawImageContainer;
    _FreeRawImageContainerFunc FreeRawImageContainer;

    // Методы для работы с изображениями и ListView
    void __fastcall AddImageToGroup(TRawImageContainer* image, int lightType);
    void __fastcall ShowImageGroup(int index);
    void __fastcall DeleteImageGroup(int index);
    void __fastcall RotateImagesInGroup(int index, bool clockwise);
    void __fastcall SaveAllImages();
    void __fastcall ClearAll();
    int __fastcall FindImageGroupByScanNumber(int scanNumber);
    void __fastcall CleanupListView();
    
    // Новые методы для работы с освещением и миниатюрами
    UnicodeString __fastcall GetLightDescription(DWORD light);
    TBitmap* __fastcall CreateThumbnail(TRawImageContainer* image);
    int __fastcall AddThumbnailToImageList(TRawImageContainer* image);
    
    // Корректировка Data/Caption элементов ListView после сдвига scanNumber
    void __fastcall RebuildListViewByImageGroups();
    
    // Новые методы для работы с файлами
    void __fastcall CreateZipArchive(const UnicodeString& sourceDir);
    
    // Вспомогательные методы
    void __fastcall InitListView();
    void __fastcall HandleKeyboardNavigation(WORD key);
    void __fastcall ReadMRZZone();  // Функция для считывания машиносчитываемой зоны
    void __fastcall SetVideoDetectionSensitivity();

public:    // User declarations
    __fastcall TForm1( TComponent* Owner );

protected:
	void __fastcall TLoadLibrary();
	void __fastcall TIntialize();
	void __fastcall TFreeLibrary();
    // Updates buttons, checkboxes etc. depending on the form state
    // e.g. connected/disconnected
    void __fastcall UpdateControls();

    // Sets the document presence color. The box color indicates whether a
    // document is in the tray or not
    void __fastcall PaintDocPresenceBox( const TColor color );

    // Log messages on the form
    void LogMessage( const wchar_t *fmt, ... ) const;
    void LogMessage( const UnicodeString& msg ) const;
    void __fastcall LogErrorCode( const eDeviceLimitations msg );

    // Result receiving callback set before the initialization
    static
    void __stdcall MyResultReceivingFunc( TResultContainer *result,
                                          uint32_t *PostAction,
                                          uint32_t *PostActionParameter );

    // Notification callback set before the initialization
    static
    void __stdcall MyNotifyFunc( intptr_t rawCode, intptr_t value );

    // Result receiving callback helpers
    void __fastcall ReceiveVisualExtOCR( const TResultContainer *result );
    void __fastcall ReceiveRawImage( const TResultContainer *result );
    void __fastcall ReceiveFileImage( const TResultContainer *result );
    void __fastcall Receive_OCR_Graphics( const TResultContainer *result );
    void __fastcall ReceiveMRZTestQuality( const TResultContainer *result );
    void __fastcall ReceiveBarcode( const TResultContainer *result );
    void __fastcall ReceiveDocTypes( const TResultContainer *result,
                                     uint32_t *PostAction,
                                     uint32_t *PostActionParameter );
    void __fastcall
    ReceiveDocTypeCandidate( const TResultContainer *result );

    void __fastcall ReceiveOCRLexAnalisys( const TResultContainer *result );
    void __fastcall ReceiveAuthenticity( const TResultContainer *result );

    // Notification receiving callback helpers
    void __fastcall NotifyDocumentReady( const bool isPlaced );
    void __fastcall NotifyScanning( const bool isScanningCompleted );
    void __fastcall NotifyDevEnumeration( const bool isEnumCompleted );
    void __fastcall NotifyDevConnected( const bool isConnected );
    void __fastcall NotifyDocCanBeRemoved();
    void __fastcall NotifyLidOpen( const bool isOpen );
    void __fastcall NotifyProcessingDone( const bool isDone );
    void __fastcall NotifyCalibInfoDownload( const int percent );
    void __fastcall NotifyLicenseExpired( const int daysSince1900 );
    void __fastcall NotifyProgress( const int percent );
    void __fastcall NotifySDKAvailable( const uint32_t version );
    void __fastcall NotifyDBAvailable( const uint32_t version );

    void __fastcall
    NotifyVideoFrame( const TVideodetectionNotification* frame );

    void __fastcall
    NotifyCompatibilityMode( const eDeviceLimitations mode );

    // Executes the command
    template< eRPRM_Commands command >
    eRPRM_ErrorCodes Call( void *params = nullptr,
                           void *result = nullptr ) const
    {
        return
            static_cast< eRPRM_ErrorCodes >(
                ExecuteCommand( command,
                                params,
                                result ) );

    }

    // Executes the command
    template< eRPRM_Commands command >
    eRPRM_ErrorCodes Call( long params, void* result = nullptr ) const
    {
        return
            static_cast< eRPRM_ErrorCodes >(
                ExecuteCommand( command,
                                reinterpret_cast< void* >( params ),
                                result ) );
    }

    // Executes the command
    template< eRPRM_Commands command >
    eRPRM_ErrorCodes Get( void* result = nullptr ) const
    {
        return
            static_cast< eRPRM_ErrorCodes >(
                ExecuteCommand( command,
                                nullptr,
                                result ) );
    }

    // A wrapper function for the _CheckResult
    template< int type, class PathFunc >
    void GetResult( const long mode, PathFunc&& getPathFor )
    {
        GetResult< type >( mode, getPathFor, []( auto ){} );
    }

    // A wrapper function for the _CheckResult
    template< const int type, class PathFunc, class ResultFunc >
    void GetResult( const long mode,
                    PathFunc&& getPathFor,
                    ResultFunc&& passResult )
    {
        const auto total = ResultTypeAvailable( type );
        LogMessage( L"%d results available of type %d "
                    L"to be read by CheckResult",
                    total,
                    type );

        for( int idx {}; idx < total; ++idx )
        {
            const UTF8String file = getPathFor( idx );

            ::DeleteFileA( file.c_str() );

            const auto hRes = CheckResult( type,
                                           idx,
                                           mode,
                                           file.c_str() );

            const auto ret = reinterpret_cast< intptr_t >( hRes );
            if( ret <= 0 )
            {
                LogMessage( L"CheckResult for idx %d returned: 0x%08X",
                            idx,
                            ret );
                continue;
            }

            // Check the file was actually saved
            const auto attr = ::GetFileAttributesA( file.c_str() );
            if( ( attr != INVALID_FILE_ATTRIBUTES ) &&
                !( attr & FILE_ATTRIBUTE_DIRECTORY ) )
            {
                LogMessage( file + " - saved." );
            }

            // Call the postprocessing if any
            passResult( hRes );
        }
    }

    // Loads PasspR function by procName
    template< typename T >
    void LoadFun( T& f, LPCSTR procName )
    {
        auto proc = ::GetProcAddress( m_hLib, procName );
        if( ! proc )
        {
            System::UnicodeString msg
            {
                "Cannot import "
            };
            msg += procName;
            throw Exception { msg };

        }
        f = reinterpret_cast< T >( proc );
    }

    // Frees previously allocated TRawImageContainer* memory
    // and clears the images container
    template< class T >
    void CleanupImageContainer( T& images )
    {
        for( auto i : images )
        {
            FreeRawImageContainer( i );
        }
        images.clear();
    }

    // Connect reader
    void __fastcall Connect();

    // Disconnect reader
    void __fastcall Disconnect();

    //Switch between different digital camera sensor modes
    void __fastcall SetWorkingVideoMode( const eRPRM_VideoModes mode );

    // Returns a path for scanning result files
    UnicodeString __fastcall GetTestDataPath() const;

    uint32_t __fastcall GetProcessingMode() const;
    void __fastcall RefreshSDKCapabilities();
    long __fastcall GetDirectQueryMode() const;
    void __fastcall DoDirectQuery( const long mode );

    void __fastcall SetUVExposure( long value );
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif

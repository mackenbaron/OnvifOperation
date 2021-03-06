//************************************
// DLL usage :
// init_DLL() first(get some memory from system).
// Get a pointer to the onvif_device_list using malloc_device_list();
// Then search_onvif_device() or add_onvif_device_manually().
// Set username password in onvif_device_list by calling set_onvif_device_authorization_information().
// Before you can use any other API, you MUST call get_onvif_device_service_addresses().
// Now you can get_onvif_device_information(), get_onvif_ipc_profiles().
// All the information you want can be found in onvif_device_list, read only please.
// If you added new onvif device, call search_onvif_device() again and you should found new device in onvif_device_list.
// Old devices' information in onvif_device_list will be preserved but as for the new devices you need to call APIs to get device information.
// When you done, please call free_device_list() and uninit_DLL() to release memory.
//************************************

//************************************
// If there is VMware adapter, please disable it;
// otherwise multicast package will end up somewhere unknown.
// Allow the program through fire wall.
//************************************

//************************************
// NOTICE:
// I try to use data lock to protect data when multithreading,
// but after some test it seems to be no effect, please consider all functions thread unsafe.
//************************************


#pragma once

#ifdef ONVIFOPERATION_EXPORTS
#define ONVIFOPERATION_API __declspec(dllexport)
#else
#define ONVIFOPERATION_API __declspec(dllimport)
#endif


enum video_encoding
{
    JPEG = 0,
    MPEG4 = 1,
    H264 = 2
};


enum audio_encoding
{
    G711 = 0,
    G726 = 1,
    AAC = 2
};


enum H264Profile
{
    Baseline = 0,
    Main = 1,
    Extended = 2,
    High = 3
};


enum ReceiverMode
{
    AutoConnect = 0,
    AlwaysConnect = 1,
    NeverConnect = 2,
    UnknownMode = 3
};


enum StreamType
{
    RTP_Unicast = 0,
    RTP_Multicast = 1
};

enum TransportProtocol
{
    UDP = 0,
    TCP = 1,
    RTSP = 2,
    HTTP = 3
};

// onvif_ipc_profile struct
typedef struct tag_onvif_ipc_profile
{

    char name[30];
    char token[30];

    struct tag_VideoSourceConfiguration
    {
        char Name[30];
        int UseCount;
        char SourceToken[30];
    }VideoSourceConfiguration;

    struct tag_AudioSourceConfiguration
    {
        char Name[30];
        int UseCount;
        char SourceToken[30];
    }AudioSourceConfiguration;

    struct tag_VideoEncoderConfiguration
    {
        char Name[30];
        int UseCount;
        enum video_encoding encoding;

        struct tag_Resolution
        {
            int Width;
            int Height;
        }Resolution;

        double Quality;

        struct tag_RateControl
        {
            int FrameRateLimit;
            int EncodingInterval;
            int BitrateLimit;
        }RateControl;

        struct tag_H264
        {
            int GovLength;
            enum H264Profile Profile;
        }H264;
    }VideoEncoderConfiguration;

    struct tag_AudioEncoderConfiguration
    {
        char Name[30];
        int UseCount;
        enum audio_encoding encoding;

        int Bitrate;
        int SampleRate;
    }AudioEncoderConfiguration;

    struct tag_MediaUri
    {
        char URI[256];
        bool InvalidAfterConnect;
        bool InvalidAfterReboot;
        __int64 Timeout;
    }MediaUri;

}onvif_ipc_profile;

typedef struct tag_onvif_device_service_address
{
    char namesapce[256];
    char xaddr[256];
    int major_version;
    int minor_version;
}onvif_device_service_address;

typedef struct tag_onvif_device_information
{
    char manufacturer[50];
    char model[50];
    char firmware_version[50];
    char serial_number[50];
    char hardware_Id[10];
    char MAC_address[50];
}onvif_device_information;

typedef struct tag_onvif_NVR_receiver
{
    char token[30];
    struct
    {
        enum ReceiverMode mode;
        char media_URI[256];

        struct
        {
            enum StreamType stream;
            enum TransportProtocol protocol;
        }stream_setup;

    }configuration;
}onvif_NVR_receiver;

typedef struct tag_onvif_device
{
    //************************************
    // For informaiton preserve purpose.
    // User MUST NOT set this content. ignore it please.
    //************************************
    bool duplicated;

    //************************************
    // IPv4 of the onvif device, set by search_onvif_device().
    // User MUST NOT set this content. Read only.
    //************************************
    char IPv4[17];

    //************************************
    // onvif device service addresses, set by get_onvif_device_service_addresses().
    // User MUST NOT set this content. ignore it please.
    //************************************
    onvif_device_service_address service_address_device_service;
    onvif_device_service_address service_address_media;
    onvif_device_service_address service_address_events;
    onvif_device_service_address service_address_PTZ;
    onvif_device_service_address service_address_imaging;
    onvif_device_service_address service_address_deviceIO;
    onvif_device_service_address service_address_analytics;
    onvif_device_service_address service_address_recording;
    onvif_device_service_address service_address_search_recording;
    onvif_device_service_address service_address_replay;
    onvif_device_service_address service_address_receiver;

    //************************************
    // device authorization information.
    // User MUST set this content by calling set_onvif_device_authorization_information()
    //************************************
    char username[50];
    char password[50];

    //************************************
    // device basic information, set by get_onvif_device_information().
    // User MUST NOT set this content. Read only.
    //************************************
    onvif_device_information device_information;

    //************************************
    // onvif device profiles, set by get_onvif_ipc_profiles().
    // User MUST NOT set this content. Read only.
    //************************************
    onvif_ipc_profile* p_onvif_ipc_profiles;
    size_t number_of_onvif_ipc_profiles;

    //************************************
    // onvif NVR receivers, set by get_onvif_nvr_receivers().
    // User MUST NOT set this content. Read only.
    //************************************
    onvif_NVR_receiver* p_onvif_NVR_receivers;
    size_t number_of_onvif_NVR_receivers;
}onvif_device;

typedef struct tag_onvif_device_list
{
    onvif_device* p_onvif_devices;
    size_t number_of_onvif_devices;

    //************************************
    // Critical section data lock, it's better to check this before you want to get some data.
    // Cast critical_section to LPCRITICAL_SECTION
    // You can lock this data struct, but don't forget to unlock it.
    //************************************
    char critical_section[70];
}onvif_device_list;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


    //************************************
    // function:  initial DLL, locate some memory; call once per process.
    // Returns:   int: 0 success, -1 failure.
    // Parameter: void.
    //************************************
    ONVIFOPERATION_API int init_DLL(void);


    //************************************
    // function:  uninitial DLL; call once per process.
    // Returns:   int: 0 success, -1 failure.
    // Parameter: void.
    //************************************
    ONVIFOPERATION_API int uninit_DLL(void);


    //************************************
    // function:  get a pointer to the device list head.
    // Returns:   a pointer to the device list head, NULL if failed.
    // Parameter: void.
    //************************************
    ONVIFOPERATION_API onvif_device_list* malloc_device_list(void);


    //************************************
    // function:  free the device list head.
    // Returns:   void.
    // Parameter: pointer to the pointer which points to the device list.
    //************************************
    ONVIFOPERATION_API void free_device_list(onvif_device_list** pp_onvif_device_list);


    //************************************
    // function:  search onvif device. if you search again, old device's information in onvif_device_list will be preserved.
    // Returns:   int: 0 success, -1 failure
    // Parameter: onvif_device_list* p_onvif_device_list: pointer get from malloc_device_list(void).
    // Parameter: int waitTime: interval for cameras to response, when > 0, gives socket recv timeout in seconds, < 0 in usec.
    //************************************
    ONVIFOPERATION_API int search_onvif_device(onvif_device_list* p_onvif_device_list, int wait_time);

    //************************************
    // function:  for those support onvif protocol but discoverable is disabled, or search can't find the device, we have to add it to list manually.
    //            old device's information in onvif_device_list will be preserved.
    // Returns:   int: 0 success, -1 failure
    // Parameter: onvif_device_list* p_onvif_device_list: pointer get from malloc_device_list(void).
    // Parameter: char* IP: the device's IP you want to add.
    //************************************
    ONVIFOPERATION_API int add_onvif_device_manually(onvif_device_list* p_onvif_device_list, char* IP);

    //************************************
    // function:  set authorization information.
    // Returns:   int: 0 success, -1 failure.
    // Parameter: onvif_device_list* p_onvif_device_list: pointer get from malloc_device_list(void).
    // Parameter: char* IP: the IPC's IP you want to operate, or you can use index get from onvif_device_list.
    // Parameter: size_t index: index of onvif_device array, if char* IP is not NULL, this parameter will be ignored, you can pass whatever into it.
    // Parameter: char* username:C-style username string.
    // Parameter: char* password:C-style password string.
    //************************************
    ONVIFOPERATION_API int set_onvif_device_authorization_information(onvif_device_list* p_onvif_device_list, char* IP, size_t index, char* username, char* password);

    //************************************
    // function:  get onvif device service addresses.
    // Returns:   int: 0 success, -1 failure.
    // Parameter: onvif_device_list* p_onvif_device_list: pointer get from malloc_device_list(void).
    // Parameter: char* IP: the IPC's IP you want to operate, or you can use index get from onvif_device_list.
    // Parameter: size_t index: index of onvif_device array, if char* IP is not NULL, this parameter will be ignored, you can pass whatever into it.
    //************************************
    ONVIFOPERATION_API int get_onvif_device_service_addresses(onvif_device_list* p_onvif_device_list, char* IP, size_t index);

    //************************************
    // function:  get onvif device information.
    // Returns:   int: 0 success, -1 failure.
    // Parameter: onvif_device_list* p_onvif_device_list: pointer get from malloc_device_list(void).
    // Parameter: char* IP: the IPC's IP you want to operate, or you can use index get from onvif_device_list.
    // Parameter: size_t index: index of onvif_device array, if char* IP is not NULL, this parameter will be ignored, you can pass whatever into it.
    //************************************
    ONVIFOPERATION_API int get_onvif_device_information(onvif_device_list* p_onvif_device_list, char* IP, size_t index);

    //************************************
    // function:  get onvif device profiles.
    // Returns:   int: 0 success, -1 failure.
    // Parameter: onvif_device_list* p_onvif_device_list: pointer get from malloc_device_list(void).
    // Parameter: char* IP: the IPC's IP you want to operate, or you can use index get from onvif_device_list.
    // Parameter: size_t index: index of onvif_device array, if char* IP is not NULL, this parameter will be ignored, you can pass whatever into it.
    //************************************
    ONVIFOPERATION_API int get_onvif_ipc_profiles(onvif_device_list* p_onvif_device_list, char* IP, size_t index);

    //************************************
    // function:  get onvif NVR receivers.
    // Returns:   int: 0 success, -1 failure.
    // Parameter: onvif_device_list* p_onvif_device_list: pointer get from malloc_device_list(void).
    // Parameter: char* IP: the IPC's IP you want to operate, or you can use index get from onvif_device_list.
    // Parameter: size_t index: index of onvif_device array, if char* IP is not NULL, this parameter will be ignored, you can pass whatever into it.
    //************************************
    ONVIFOPERATION_API int get_onvif_nvr_receivers(onvif_device_list* p_onvif_device_list, char* IP, size_t index);

#ifdef __cplusplus
}
#endif // __cplusplus
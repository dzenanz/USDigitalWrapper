#include "SEIDrv.h"
#include <iostream>
#include <string>
#include <Windows.h>

std::string CLRtoSTL(System::String^ string)
{
    System::IntPtr ansiStr = System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(string);
    std::string outStr = (const char*)ansiStr.ToPointer();
    System::Runtime::InteropServices::Marshal::FreeHGlobal(ansiStr);
    return outStr;
}

ref struct Globals
{
    static System::Collections::Generic::List<USDigital::A2^> encoders;
    static System::Collections::Generic::List<unsigned> counts;
};

void printUpdatedPositions()
{
    for (int i = 0; i < Globals::encoders.Count; i++)
    {
        unsigned newCount = Globals::encoders[i]->Count;
        USDigital::A2::A2EncoderStatus status; // The GetCount(count) signature does not work properly
        Globals::encoders[i]->GetCount(newCount, status); // so we use GetCount(count, status)
        if (newCount != Globals::counts[i])
        {
            std::cout << "Encoder " << i << ": " << newCount << std::endl;
            Globals::counts[i] = newCount;
        }
    }
}

void CountChanged(USDigital::DevicePropertyEventArgs^ e)
{
    unsigned count = (unsigned)e->Value;
    int index = -1;
    for (int i = 0; i < Globals::encoders.Count; i++)
    {
        if (Globals::encoders[i] == e->Device)
        {
            index = i;
            break;
        }
    }
    std::cout << index << ": " << count << std::endl;
    // Globals::counts[index] = count; // synchronize with printUpdatedPositions
}


void enumerateEncodersAll()
{
    char lpTargetPath[5000]; // buffer to store the path of the COMPORTS
    for (int i = 0; i < 255; i++)
    {
        std::string str = "COM" + std::to_string(i);
        DWORD test = QueryDosDevice(str.c_str(), lpTargetPath, 5000);

        if (test != 0) // port exists
        {
            std::cout << str << ": " << lpTargetPath << std::endl;
            enumerateEncoders(str.c_str());
        }
    }
}

void enumerateEncoders(const char* comPort)
{
    System::String^ comPortNet = gcnew System::String(comPort);

    USDigital::SEIBusManager mgr;
    USDigital::SEIBus^ mSEIBus = mgr.GetBus(comPortNet);
    mSEIBus->Initialize(USDigital::InitializationFlags::AUTOASSIGN
        + USDigital::InitializationFlags::NORESET
        + USDigital::InitializationFlags::REINITIALIZE);

    //now inspect the devices array
    for (int i = 0; i < mSEIBus->Devices->Length; i++)
    {
        USDigital::A2^ a2Dev = (USDigital::A2^)mSEIBus->Devices[i];
        Globals::encoders.Add(a2Dev);
        a2Dev->OnCountChange += gcnew USDigital::DevicePropertyEventHandler(CountChanged);
        std::cout << "Encoder " << i << ": " << CLRtoSTL(a2Dev->Product) << std::endl;
        std::cout << "\tAddress: " << (int)a2Dev->Address << std::endl;
        std::cout << "\tFirmwareVersion: " << CLRtoSTL(a2Dev->FirmwareVersion) << std::endl;
        std::cout << "\tManufacturedDate: " << CLRtoSTL(a2Dev->ManufacturedDate.ToString()) << std::endl;

        //A2-specific methods
        std::cout << "\tUsesSEIAdapter: " << a2Dev->UsesSEIAdapter << std::endl;
        std::cout << "\tCount: " << a2Dev->Count << std::endl;
        Globals::counts.Add(a2Dev->Count);
    }
}

void removeCallbacks()
{
    for (int i = 0; i < Globals::encoders.Count; i++)
    {
        Globals::encoders[i]->OnCountChange -= gcnew USDigital::DevicePropertyEventHandler(CountChanged);
    }
}

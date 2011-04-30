#pragma once

extern "C"
{
    // for a specific COM port
    void enumerateEncoders(const char* comPort);

    // for all COM ports
    void enumerateEncodersAll();

    void printUpdatedPositions();

    void removeCallbacks();
}
#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE hSerial;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};

    // เปิดพอร์ต COM20
    hSerial = CreateFile(
        "\\\\.\\COM20",            // ชื่อพอร์ต
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    if (hSerial == INVALID_HANDLE_VALUE) {
        printf("Error opening COM port\n");
        return 1;
    }

    // ตั้งค่า DCB (Device Control Block)
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        printf("Error getting state\n");
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_9600; // ตั้งค่าความเร็ว Baud rate
    dcbSerialParams.ByteSize = 8;        // จำนวนบิตต่อไบต์ (8)
    dcbSerialParams.StopBits = ONESTOPBIT; // จำนวน Stop bit (1)
    dcbSerialParams.Parity   = NOPARITY;   // Parity (ไม่มี)
    dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE; // เปิดใช้งาน DTR
    dcbSerialParams.fRtsControl = RTS_CONTROL_ENABLE; // เปิดใช้งาน RTS

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        printf("Error setting state\n");
        CloseHandle(hSerial);
        return 1;
    }

    // ตั้งค่า timeout
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        printf("Error setting timeouts\n");
        CloseHandle(hSerial);
        return 1;
    }

    // ข้อความที่ต้องการส่ง
    const char *msg = "bootld\r\n";
    DWORD bytes_written;

    // ส่งข้อความไปยัง COM20
    if (!WriteFile(hSerial, msg, strlen(msg), &bytes_written, NULL)) {
        printf("Error writing to COM port\n");
        CloseHandle(hSerial);
        return 1;
    }

    // ปิดพอร์ต COM
    CloseHandle(hSerial);

    // ปิดหน้าจอถ้าส่งสำเร็จ
    system("cls");

    return 0;
}

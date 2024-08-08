
# ตัวอย่างการสื่อสารแบบ Serial กับพอร์ต COM บน Windows

โปรแกรมนี้เป็นตัวอย่างการตั้งค่าและใช้การสื่อสารแบบ serial บนระบบปฏิบัติการ Windows โดยใช้ WinAPI โปรแกรมจะเปิดพอร์ต COM กำหนดค่าพารามิเตอร์ของพอร์ต serial และส่งข้อความไปยังพอร์ตนั้น

## ข้อกำหนดเบื้องต้น

- ระบบปฏิบัติการ Windows
- MinGW หรือคอมไพเลอร์ C อื่น ๆ สำหรับ Windows

## วิธีการคอมไพล์

ให้ใช้คำสั่งต่อไปนี้ใน Command Prompt หรือ Terminal:  

```sh
gcc -o serial_communication serial_communication.c -lws2_32
```

## วิธีการใช้งาน

1. เปิดโปรแกรม Command Prompt หรือ Terminal
2. รันโปรแกรมที่คอมไพล์แล้วด้วยคำสั่ง:

```sh
serial_communication
```

## การทำงานของโปรแกรม

โปรแกรมนี้จะทำงานดังนี้:

1. เปิดพอร์ต COM20
2. ตั้งค่าพารามิเตอร์ของพอร์ต serial (Baud rate, Byte size, Stop bit, Parity)
3. ตั้งค่า timeout สำหรับการอ่านและการเขียน
4. ส่งข้อความ "bootld
" ไปยังพอร์ต COM20
5. ปิดพอร์ต COM

หากโปรแกรมทำงานสำเร็จ หน้าจอจะถูกล้างหลังจากส่งข้อความเรียบร้อยแล้ว

## โค้ด

```c
#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE hSerial;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};

    // เปิดพอร์ต COM20
    hSerial = CreateFile(
        "\\.\COM20",            // ชื่อพอร์ต
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
```

#include "LbootOutRecord.h"
#include <QStringList>


LbootOutRecord::LbootOutRecord(QString line) {
    QStringList strList = line.split(":", QString::SkipEmptyParts);
    if (strList.size() >= 2) {
        m_stage = strList[0].trimmed();
        m_status = strList[1].trimmed();
        for (int i=2; i < strList.size(); i++) {
            QStringList paramStr = strList[i].split("=");
            if (paramStr.size() >= 2) {
                m_params[paramStr[0].trimmed()] = paramStr[1].trimmed();
            }
        }
    }
}

QString LbootOutRecord::stageName() {
    if (m_stage == stageCheckParams())
        return tr("Check input parameters");
    if (m_stage == stageOpenDevice())
        return tr("Open device");
    if (m_stage == stageCheckInfo())
        return tr("Check device information");
    if (m_stage == stageGetDevMode())
        return tr("Get device mode");
    if (m_stage == stageCheckDevMode())
        return tr("Check device mode");
    if (m_stage == stageGetBootVersion())
        return tr("Get bootloader version");
    if (m_stage == stageGetDevFlags())
        return tr("Get device state flags");
    if (m_stage == stageGetBootFeatures())
        return tr("Get bootloader features");
    if (m_stage == stageCheckBootVer())
        return tr("Check bootloader version");
    if (m_stage == stageWriteProtData())
        return tr("Write protected data");
    if (m_stage == stageWriteFirmware())
        return tr("Write firmware");
    if (m_stage == stageWriteSign())
        return tr("Write signature/hash");
    if (m_stage == stageRecovery())
        return tr("Recovery main firmware");
    if (m_stage == stageStartApp())
        return tr("Start application");
    return QString();
}

int LbootOutRecord::errorCode() const {
    int ret = 0;
    if (status() == statusError()) {
        ret = param(paramErrCode()).toInt();
    }
    return ret;
}

QString LbootOutRecord::errorDescr() const {
    int err_code = errorCode();
    switch (err_code) {
        case 0:
            return tr("Done successfully");
        case LBOOT_ERR_INVALID_OPTION:
            return tr("Invalid input parameter");
        case LBOOT_ERR_INSUFFICIENT_ARGUMENTS:
            return tr("Insufficient input arguments");
        case LBOOT_ERR_INVALID_INTERFACE:
            return tr("Specified invalid interface");
        case LBOOT_ERR_FIRMWARE_FILE_OPEN:
            return tr("Cannot open firmware file");
        case LBOOT_ERR_SIGNATURE_FILE_OPEN:
            return tr("Cannot open signature file");
        case LBOOT_ERR_HASH_FILE_OPEN:
            return tr("Cannot open hash file");
        case LBOOT_ERR_MEMORY_ALLOC:
            return tr("Memory allocation error");
        case LBOOT_ERR_ALLOC_RESOURCE:
            return tr("Resource allocation error");
        case LBOOT_ERR_DEVICE_NOT_FOUND:
            return tr("Bootloader device not found");
        case LBOOT_ERR_IOCTRL_FAILED:
            return tr("IO request failed");
        case LBOOT_ERR_INVALID_DEVICE:
            return tr("Invalid device name");
        case LBOOT_ERR_INVALID_SERIAL:
            return tr("Invalid serial number");
        case LBOOT_ERR_INVALID_DEVMODE:
            return tr("Device not in bootloader mode");
        case LBOOT_ERR_UNSUP_FLAG_DONT_START:
            return tr("Bootloader doesn't support burn withot start application");
        case LBOOT_ERR_UNSUP_FLAG_RECOVERY:
            return tr("Bootloader doesn't support recovery operation");
        case LBOOT_ERR_INVALID_OUTPUT_FMT:
            return tr("Invalid output format specified");
        case LBOOT_ERR_UNSUP_WR_PROT_DATA:
            return tr("Bootloader doesn't support protection data");
        case LBOOT_ERR_PROT_DATA_FILE_OPEN:
            return tr("Cannot open file with protected data");
        case LBOOT_ERR_MBRTU_INVALID_BAUDRATE:
            return tr("Invalid rs baudrate specified");
        case LBOOT_ERR_MBRTU_INVALID_PARITY:
            return tr("Invalid rs parity specified");
        case LBOOT_ERR_MBRTU_INVALID_ADDRESS:
            return tr("Invalid modbus address specified");
        case LBOOT_ERR_MBRTU_CREATE_CONTEXT:
            return tr("Cannot create modbus context");
        case LBOOT_ERR_MBRTU_SET_ADDR:
            return tr("Cannot set modbus address");
        case LBOOT_ERR_MBRTU_PORT_OPEN:
            return tr("Cannot open modbus port");
        case LBOOT_ERR_READ_REGISTERS:
            return tr("Cannot read modbus registers");
        case LBOOT_ERR_WRITE_REGISTERS:
            return tr("Cannot write modbus registers");
        case LBOOT_ERR_TFTP_SOCK_CREATE:
            return tr("Cannot create socket for TFTP operations");
        case LBOOT_ERR_TFTP_SEND:
            return tr("Send TFTP packet error");
        case LBOOT_ERR_TFTP_RECV:
            return tr("Receive TFTP packet error");
        case LBOOT_ERR_TFTP_RECV_PKT_FMT:
            return tr("Invalid format of received TFTP packet");
        case LBOOT_ERR_TFTP_TIMEOUT:
            return tr("TFTP packet response timeout");
        case LBOOT_ERR_TFTP_IP_ADDR:
            return tr("Invalid TFTP host address specified");
        case LBOOT_ERR_TFTP_UDP_PORT:
            return tr("Invalid UDP port for TFTP interface specified");
        case LBOOT_ERR_TFTP_BLOCK_SIZE:
            return tr("Invalid TFTP block size specified");
        case LBOOT_ERR_TFTP_ERR_PACKET:
            return tr("Receive TFTP error packet");
        case LBOOT_ERR_TFTP_RX_OVERFLOW:
            return tr("Received TFTP packets overflow");
        case LBOOT_ERR_TFTP_RX_INSUF_SIZE:
            return tr("Receive insufficient data over TFTP");
        case LBOOT_ERR_TFTP_PACKET_SKIP:
            return tr("Received not expected TFTP packet");
        case LBOOT_ERR_TFTP_WSA_INIT:
            return tr("Socket system initialization error");
        case LBOOT_ERR_UNCORRECT_SIGN:
            return tr("Signature check failed");
        case LBOOT_ERR_UNSTABLE_RECOV_WR:
            return tr("Cannot write unstable firmware as reserve copy");
        case LBOOT_ERR_FIRMWARE_FOR_INVALID_DEVICE:
            return tr("Firmware file is invalid or for another device type");
        case LBOOT_ERR_UNCORRECT_START_SIGN:
            return tr("Incorrect sign for start write firmware");
        case LBOOT_ERR_WR_DISABLED:
            return tr("Write this type of firmware is disabled");
        case LBOOT_ERR_FLASH_WRITE:
            return tr("Flash memory write error");
        case LBOOT_ERR_UNCORRECT_FIRM_RESET_ADDR:
            return tr("Invalid start address in firmware");
        case LBOOT_ERR_INVALID_REQ_DATA_LENGTH:
            return tr("Invalid request data length");
        case LBOOT_ERR_INVALID_OPERATION:
            return tr("Invalid command");
        case LBOOT_ERR_UNSUP_UNPACK:
            return tr("Unpack operation is not supported");
        case LBOOT_ERR_UNPACK_NOT_COMPLETE:
            return tr("Unpack operation is not complete");
        case LBOOT_ERR_CHECK_WRITE_FIRM:
            return tr("Firmware integrity check error");
        case LBOOT_ERR_INVALID_SECTOR:
            return tr("Invalid flash write sector");
        case LBOOT_ERR_FIRM_WITHOUT_SIGN_DISABLED:
            return tr("Write firmware without signature is disabled");
        case LBOOT_ERR_INVALID_HASH:
            return tr("Invalid firmware hash");
        case LBOOT_ERR_DEVELOP_FIRM_DISABLE:
            return tr("Write development firmware is disabled");
        case LBOOT_ERR_INVALID_CMD_CODE:
            return tr("Unsupported special command code");
        case LBOOT_ERR_INVALID_CMD_PARAMS:
            return tr("Invalid special command parameters");
        case LBOOT_ERR_INVALID_CMD_SIGN:
            return tr("Invalid special command sign");
        case LBOOT_ERR_RESERV_COPY_NOT_PRESENT:
            return tr("Reserve copy is't present");
        case LBOOT_ERR_APPLICATION_NOT_PRESENT:
            return tr("Main application is't present");
        case LBOOT_ERR_INVALID_APPL_SIZE:
            return tr("Invalid firmware size");
        case LBOOT_ERR_PROT_DATA_INVALID_SIZE:
            return tr("invalid protection block size");
        case LBOOT_ERR_PROT_DATA_WR_DISABLED:
            return tr("Write protected block is disabled");
        case LBOOT_ERR_PROT_DATA_CHECK:
            return tr("Protected block integrity check error");
        case LBOOT_ERR_INVALID_REG_ADDR:
            return tr("Invalid register address");
        case LBOOT_ERR_ABORTED_BY_ANOTHER_REQ:
            return tr("Operation aborted by another request");
        case LBOOT_ERR_INVALID_XZ_FORMAT:
            return tr("Invalid xz archive format");
        case LBOOT_ERR_INVALID_SIGN_FORMAT:
            return tr("Invalid signature format");
        case LBOOT_ERR_FLASH_COMPARE:
            return tr("Flash compare operation failed");
        case LBOOT_ERR_FLASH_ERASE:
            return tr("Flash erase error");
        case LBOOT_ERR_CUR_FIRMWARE_NOT_SELECTED:
            return tr("Current firmware is not selected");
        case LBOOT_ERR_UNKNOWN_FIRMWARE_TYPE:
            return tr("Unsupported firmware type");


        case LCRYPT_ERR_PGP_UNSUF_DATA:
            return tr("Received not complete signature packet");
        case LCRYPT_ERR_PGP_SIGN_FORMAT:
            return tr("Invalid signature packet format");
        case LCRYPT_ERR_PGP_SIGN_UNSUPPORT_CRYPT_ALG:
            return tr("Unsupported signature cryptographic algorithm");
        case LCRYPT_ERR_PGP_SIGN_UNSUPPORT_HASH_ALG:
            return tr("Unsupported signature hash algorithm");
        default:
            return param(paramErrString());
    }
}

QString LbootOutRecord::errorTypeDescr() const {
    QString type = param(paramErrType());
    if (type == errorTypeUtility())
        return tr("Utility error");
    if (type == errorTypeDevice())
        return tr("Device error");
    if (type == errorTypeIface())
        return tr("Interface error");
    return QString();
}

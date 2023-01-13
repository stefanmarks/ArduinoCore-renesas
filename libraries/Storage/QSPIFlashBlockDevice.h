/* ########################################################################## */
/* - File: QSPIQSPIFlashBlockDevice.h
   - Copyright (c): 2023 Arduino srl.  All right reserved.
   - Author: Daniele Aimo (d.aimo@arduino.cc)

     This library is free software; you can redistribute it and/or
     modify it under the terms of the GNU Lesser General Public
     License as published by the Free Software Foundation; either
     version 2.1 of the License, or (at your option) any later version.

     This library is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     Lesser General Public License for more details.

     You should have received a copy of the GNU Lesser General Public
     License along with this library; if not, write to the Free Software
     Foundation, Inc.,51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA */
/* ########################################################################## */
#ifndef ARDUINO_QSIP_FLASH_BLOCK_DEVICE
#define ARDUINO_QSPI_FLASH_BLOCK_DEVICE

/* base class for block devices */
#include "blockDevice.h"

/* Arduino.h to include the defines of the flash type LP or HP*/
#include "Arduino.h"

#include "r_qspi.h"


/* QSPI flash page number to be written */
#define PAGE_FIRST                      (0U)

/* QSPI flash page Size */
#define PAGE_WRITE_SIZE                 (256U)

/* QSPI flash address through page*/
#define QSPI_FLASH_ADDRESS(page_no)     (uint8_t *) (QSPI_DEVICE_START_ADDRESS + (page_no * PAGE_WRITE_SIZE))

/* Status register pay-load */
#define STATUS_REG_PAYLOAD              {0x01,0x40,0x00}

/* data written to status register */
#define SET_SREG_VALUE                  (0x40)

/* sector size of QSPI flash device */
#define SECTOR_SIZE                     (4096U)

/* one byte data transfer */
#define ONE_BYTE                        (0x01)

/* SREG pay-load size */
#define SREG_SIZE                       (0x03)

/* default memory value */
#define DEFAULT_MEM_VAL                 (0xFF)

/* -------------------------------------------------------------------------- */
/* CLASS QSPIFlashBlockDevice - to access micro internal flash                    */
/* -------------------------------------------------------------------------- */

class QSPIFlashBlockDevice : public CBlockDevice {
private:
  
  BdAddr_t base_address;
  BdSize_t size;
  BdSize_t read_block_size;
  BdSize_t erase_block_size;
  BdSize_t write_block_size;

  bool is_address_correct(BdAddr_t add);

  qspi_instance_ctrl_t  ctrl;
  spi_flash_cfg_t       cfg;
  qspi_extended_cfg_t   ext_cfg;
  pin_t ck;
  pin_t cs;
  pin_t io0;
  pin_t io1;
  pin_t io2;
  pin_t io3;

  fsp_err_t get_flash_status();

  spi_flash_erase_command_t g_qspi_erase_command_list[4] =
  {
    { .command = 0x20, .size = 4096 },
    { .command = 0x52, .size = 32768 },
    { .command = 0xD8, .size = 65536 },
    { .command = 0xC7, .size = SPI_FLASH_ERASE_SIZE_CHIP_ERASE },
  };

public:
  QSPIFlashBlockDevice( pin_t ck, 
                        pin_t cs, 
                        pin_t io0, 
                        pin_t io1, 
                        pin_t io2, 
                        pin_t io3);
  

  QSPIFlashBlockDevice(QSPIFlashBlockDevice const&) = delete;
  void operator=(QSPIFlashBlockDevice const&) = delete;
  virtual ~QSPIFlashBlockDevice(); 

  virtual int open() override;
  virtual int close() override;
  virtual int read(void *buffer, BdAddr_t addr, BdSize_t size) override;
  virtual int write(const void *buffer, BdAddr_t addr, BdSize_t size) override;
  virtual int erase(BdAddr_t addr, BdSize_t size) override;
  virtual BdSize_t getWriteBlockSize() const override;
  virtual BdSize_t getEraseBlockSize() const override;
  virtual BdSize_t getReadBlockSize() const override;
  virtual BdSize_t getTotalSize() const override;

};



#endif
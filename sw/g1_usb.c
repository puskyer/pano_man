#include "global.h"
#include "reg.h"
#include "isp1760-regs.h"

#define RW_TEST_VALUE   0x5555aaaa

static u32 isp1760_read32(u32 reg);
static u32 isp1760_write32(u32 reg,u32 Value);

static u32 isp1760_read32(u32 reg)
{
   uint16_t Lsb;
   uint16_t Msb;

   Lsb = REG16_RD(USB_1760_ADR + reg);
   Msb = REG16_RD(USB_1760_ADR + reg + 2);

   return Lsb + (Msb << 16);
}

static u32 isp1760_write32(u32 reg,u32 Value)
{
   REG16_WR(USB_1760_ADR + reg,(u16) (Value & 0xffff));
   REG16_WR(USB_1760_ADR + reg + 2,(u16) ((Value >> 16) & 0xffff));
}

// Return 1 if USB controller is detected
int UsbProbe(void)
{
   int Ret = FALSE;  // Assume the worse
   static int Toggle = 0;

   do {
      isp1760_write32(HC_SCRATCH_REG,RW_TEST_VALUE);

      if((isp1760_read32(HC_CHIP_ID_REG) & 0xffff) != 0x1761) {
         break;
      }
#if 1
      if(isp1760_read32(HC_SCRATCH_REG) != RW_TEST_VALUE) {
         break;
      }

      isp1760_write32(HC_SCRATCH_REG,~RW_TEST_VALUE);

      if(isp1760_read32(HC_SCRATCH_REG) != ~RW_TEST_VALUE) {
         break;
      }
#endif
      Ret = TRUE;

      Toggle = Toggle ? 0 : 1;
      REG_WR(LED_CONFIG_ADR,Toggle);

   } while(FALSE);

   return Ret;
}



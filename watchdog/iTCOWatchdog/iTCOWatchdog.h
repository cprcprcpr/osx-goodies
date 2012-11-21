#include "Various.h"

#define drvid "[iTCOWatchdog] "

#define DEFAULT_TIMEOUT    30

#define ITCO_BASE   (LPCNub->acpi_tco.start)
#define ITCO_SMIEN  (LPCNub->acpi_smi.start)
#define ITCO_ST1    (ITCO_BASE + 0x04)
#define ITCO_ST2    (ITCO_BASE + 0x06)
#define ITCO_RL     (ITCO_BASE + 0x00)
#define ITCO1_TM    (ITCO_BASE + 0x01)
#define ITCO2_TM    (ITCO_BASE + 0x12)
#define ITCO_CR     (ITCO_BASE + 0x08)
#define ITCO_CR_PRESERVE   0x0200
#define ITCO_SMIEN_ENABLE  0x2000
#define ITCO_SMIEN_ST      ITCO_SMIEN_ENABLE 
#define ITCO_RLD_TMR_MIN   0x04
#define ITCO1_RLD_TMR_MAX  0x03f
#define ITCO2_RLD_TMR_MAX  0x3ff
/* TCO1 */
#define ITCO_SECOND_TO_ST  0x0002 /* Ran down twice */
#define ITCO_TMR_HALT      0x0800
/* TCO2 */
#define ITCO_BOOT_ST       0x0004 /* Failed to come out of reset */
#define ITCO_TIMEOUT_ST    0x0008

#define ICHLPC_GEN_STA           0xd4
#define ICHLPC_GEN_STA_NO_REBOOT 0x02
#define ICHLPC_GCS_NO_REBOOT     0x20

class iTCOWatchdog: public super {
    OSDeclareDefaultStructors(iTCOWatchdog)
private:
    OSDictionary *conf;
    IOPCIDevice *fPCIDevice;
    IOSimpleLock *lock;
    struct {
        IODeviceMemory *range;
        IOMemoryMap *map;
        void *vaddr;
    } GCSMem;
    
    MyLPC *LPCNub;
    bool Auto, SMIEnabled, init_stage;
    UInt32 Timeout;
    
    void clearStatus();
    bool allowReboots();
    //void disableReboots();
    void reloadTimer();
#if defined DEBUG
    int readTimeleft();
#endif
protected:
    virtual bool init(OSDictionary *);
    
    virtual IOService* probe (IOService* provider, SInt32* score);
    virtual void free(void);
    
    virtual bool start(IOService *provider);
    virtual void stop(IOService *provider);
public:
    /* Interface */
    void tcoWdSetTimer(UInt32);
    void tcoWdDisableTimer();
    void tcoWdEnableTimer();
    void tcoWdLoadTimer();
};
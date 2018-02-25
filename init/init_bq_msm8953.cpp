/*
   Copyright (c) 2016, The CyanogenMod Project
   Copyright (c) 2017, The LineageOS Project

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <cstdio>

#include <android-base/strings.h>
#include <sys/sysinfo.h>

#include "vendor_init.h"
#include "property_service.h"
#include "log.h"
#include "util.h"

using android::base::Trim;

bool device_pro = false;

static void init_alarm_boot_properties()
{
    char const* boot_reason_file = "/proc/sys/kernel/boot_reason";
    char const* power_off_alarm_file = "/persist/alarm/powerOffAlarmSet";
    std::string boot_reason;
    std::string power_off_alarm;
    std::string tmp = property_get("ro.boot.alarmboot");

    if (read_file(boot_reason_file, &boot_reason)
        && read_file(power_off_alarm_file, &power_off_alarm))
    {
        /*
         * Setup ro.alarm_boot value to true when it is RTC triggered boot up
         * For existing PMIC chips, the following mapping applies
         * for the value of boot_reason:
         *
         * 0 -> unknown
         * 1 -> hard reset
         * 2 -> sudden momentary power loss (SMPL)
         * 3 -> real time clock (RTC)
         * 4 -> DC charger inserted
         * 5 -> USB charger insertd
         * 6 -> PON1 pin toggled (for secondary PMICs)
         * 7 -> CBLPWR_N pin toggled (for external power supply)
         * 8 -> KPDPWR_N pin toggled (power key pressed)
         */
        if ((Trim(boot_reason) == "3" || tmp == "true") && Trim(power_off_alarm) == "1")
            property_set("ro.alarm_boot", "true");
        else
            property_set("ro.alarm_boot", "false");
    }
}

/*
 * Devices are not much different from each other. Let's try to distinguish them by the amount of
 * RAM. If you know a better way, replace it.
 *
 * @author BeYkeRYkt (15-11-2017)
 */
void check_device()
{
    struct sysinfo sys;

    sysinfo(&sys);

    if (sys.totalram > 2048ull * 1024 * 1024) // 3GB
    {
        /* Bardock */
        device_pro = false;
    }
    else
    {
        /* Bardock Pro */
        device_pro = true;
    }
}


void load_bardock()
{
    property_override("ro.product.model", "Aquaris X");
    property_override("ro.build.product", "bardock");
    property_override("ro.product.device", "bardock");
    property_override("ro.build.description", "bardockpro-user 7.1.1 NMF26F 1.4.3_20170809-1111 release-keys");
    property_override("ro.build.fingerprint", "bq/bardock-pro/bardock-pro:7.1.1/NMF26F/1502270114:user/release-keys");
}

void load_bardockpro()
{
    property_override("ro.product.model", "Aquaris X Pro");
    property_override("ro.build.product", "bardockpro");
    property_override("ro.product.device", "bardock-pro");
    property_override("ro.build.description", "bardockpro-user 7.1.1 NMF26F 1.4.3_20170809-1111 release-keys");
    property_override("ro.build.fingerprint", "bq/bardock-pro/bardock-pro:7.1.1/NMF26F/1502270114:user/release-keys");
}

void vendor_load_properties()
{
    check_device();
    if (device_pro)
    {
        /* I think, it's pro */
        load_bardockpro();
    }
    else
    {
        load_bardock();
    }

    init_alarm_boot_properties();
}

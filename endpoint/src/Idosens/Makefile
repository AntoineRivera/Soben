#
#  author : rivera
#  date : 01/2023
#  file : Makefile
#

# name of your application
APPLICATION = capteur_porte

# Use the ST B-L072Z-LRWAN1 board by default:
EXTERNAL_BOARD_DIRS ?= $(CURDIR)../../RIOT/RIOT/boards
BOARD ?= idosens_sensor

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)../../RIOT

# Default radio driver is Semtech SX1276 (used by the B-L072Z-LRWAN1 board)
DRIVER ?= sx1276

# Default region is Europe and default band is 868MHz
LORA_REGION ?= EU868

# Include the Semtech-loramac package
USEPKG += semtech-loramac
USEPKG += cayenne-lpp


# Include drivers and modules
USEMODULE += $(DRIVER)
USEMODULE += fmt
USEMODULE += ztimer_sec
USEMODULE += ztimer_msec
USEMODULE += printf_float
USEMODULE += ztimer_no_periph_rtt
# Watchdog module
USEMODULE += xtimer
USEMODULE += shell
# Sensors
USEMODULE += at30tse75x

# Watchdog required
FEATURES_REQUIRED += periph_wdt
# RTC module
FEATURES_OPTIONAL += periph_rtc


# Pass these enviroment variables to Lora
DEVEUI ?= FD80E7CDDFC08DB5
APPEUI ?= 0000000000000000
APPKEY ?= 53848BCB7F4A28DE8F924A4FF48B64FC

DOCKER_ENV_VARS += DEVEUI
DOCKER_ENV_VARS += APPEUI
DOCKER_ENV_VARS += APPKEY

# Time in seconds between two transmits : default is 5 minutes
TXPERIOD ?= (300U)






# INCLUDES
INCLUDES += -I$(CURDIR)/include

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Use warning as warning if WERROR = 0
WERROR ?= 0
# Use warning as error if WERROR = 1
#WERROR ?= 1


include $(RIOTBASE)/Makefile.include


ifndef CONFIG_KCONFIG_USEMODULE_LORAWAN
  # OTAA compile time configuration keys
  CFLAGS += -DCONFIG_LORAMAC_APP_KEY_DEFAULT=\"$(APPKEY)\"
  CFLAGS += -DCONFIG_LORAMAC_APP_EUI_DEFAULT=\"$(APPEUI)\"
  CFLAGS += -DCONFIG_LORAMAC_DEV_EUI_DEFAULT=\"$(DEVEUI)\"
endif

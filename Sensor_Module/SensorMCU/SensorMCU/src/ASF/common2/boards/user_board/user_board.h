/**
 * \file
 *
 * \brief User board definition template
 *
 */

 /* This file is intended to contain definitions and configuration details for
 * features and devices that are available on the board, e.g., frequency and
 * startup time for an external crystal, external memory devices, LED and USART
 * pins.
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef USER_BOARD_H
#define USER_BOARD_H

#include <conf_board.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup group_common_boards
 * \defgroup user_board_group User board
 *
 * @{
 */

void system_board_init(void);

/** Name string macro */
#define BOARD_NAME                "USER_BOARD"

/** @} */

#ifdef __cplusplus
}
#endif

/** \name LED0 definitions
 *  @{ */
#define LED0_PIN                  PIN_PA08
#define LED0_ACTIVE               true
#define LED0_INACTIVE             !LED0_ACTIVE
#define LED0_GPIO                 LED0_PIN
/** @} */

/** \name SW0 definitions
 *  @{ */
#define SW0_PIN                   PIN_PA05
#define SW0_ACTIVE                false
#define SW0_INACTIVE              !SW0_ACTIVE
#define SW0_EIC_PIN               PIN_PA05A_EIC_EXTINT5
#define SW0_EIC_MUX               MUX_PA05A_EIC_EXTINT5
#define SW0_EIC_PINMUX            PINMUX_PA05A_EIC_EXTINT5
#define SW0_EIC_LINE              5
/** @} */

/** \name SW1 definitions
 *  @{ */
#define SW1_PIN                   PIN_PA09
#define SW1_ACTIVE                false
#define SW1_INACTIVE              !SW1_ACTIVE
#define SW1_EIC_PIN               PIN_PA09A_EIC_EXTINT7
#define SW1_EIC_MUX               MUX_PA09A_EIC_EXTINT7
#define SW1_EIC_PINMUX            PINMUX_PA09A_EIC_EXTINT7
#define SW1_EIC_LINE              5
/** @} */

/**
 * \name LED #0 definitions
 *
 * Wrapper macros for LED0, to ensure common naming across all Xplained
 * boards.
 *
 *  @{ */
#define LED_0_NAME                "LED0 (yellow)"
#define LED_0_PIN                 LED0_PIN
#define LED_0_ACTIVE              LED0_ACTIVE
#define LED_0_INACTIVE            LED0_INACTIVE
/** @} */

/** Number of on-board LEDs */
#define LED_COUNT                 1

/**
 * \name Button #0 definitions
 *
 * Wrapper macros for SW0, to ensure common naming across all Xplained
 * boards.
 *
 *  @{ */
#define BUTTON_0_NAME             "SW0"
#define BUTTON_0_PIN              SW0_PIN
#define BUTTON_0_ACTIVE           SW0_ACTIVE
#define BUTTON_0_INACTIVE         SW0_INACTIVE
#define BUTTON_0_EIC_PIN          SW0_EIC_PIN
#define BUTTON_0_EIC_MUX          SW0_EIC_MUX
#define BUTTON_0_EIC_PINMUX       SW0_EIC_PINMUX
#define BUTTON_0_EIC_LINE         SW0_EIC_LINE
/** @} */




/** Number of on-board buttons */
#define BUTTON_COUNT 1


/** \name Embedded debugger CDC Gateway USART interface definitions
 * @{
 */
//#define USE_DEV_BOARD
#ifdef USE_DEV_BOARD
#define EDBG_CDC_MODULE              SERCOM2
#define EDBG_CDC_SERCOM_MUX_SETTING  USART_RX_3_TX_2_XCK_3
#define EDBG_CDC_SERCOM_PINMUX_PAD0  PINMUX_UNUSED
#define EDBG_CDC_SERCOM_PINMUX_PAD1  PINMUX_UNUSED
#define EDBG_CDC_SERCOM_PINMUX_PAD2  PINMUX_PA10D_SERCOM2_PAD2
#define EDBG_CDC_SERCOM_PINMUX_PAD3  PINMUX_PA11D_SERCOM2_PAD3
#define EDBG_CDC_SERCOM_DMAC_ID_TX   SERCOM2_DMAC_ID_TX
#define EDBG_CDC_SERCOM_DMAC_ID_RX   SERCOM2_DMAC_ID_RX


#else
#define EDBG_CDC_MODULE              SERCOM1
#define EDBG_CDC_SERCOM_MUX_SETTING  USART_RX_3_TX_2_XCK_3
#define EDBG_CDC_SERCOM_PINMUX_PAD0  PINMUX_UNUSED
#define EDBG_CDC_SERCOM_PINMUX_PAD1  PINMUX_UNUSED
#define EDBG_CDC_SERCOM_PINMUX_PAD2  PINMUX_PA24C_SERCOM1_PAD2
#define EDBG_CDC_SERCOM_PINMUX_PAD3  PINMUX_PA25C_SERCOM1_PAD3
#define EDBG_CDC_SERCOM_DMAC_ID_TX   SERCOM1_DMAC_ID_TX
#define EDBG_CDC_SERCOM_DMAC_ID_RX   SERCOM1_DMAC_ID_RX
#endif
/** @} */
/**
 * \brief Turns off the specified LEDs.
 *
 * \param led_gpio LED to turn off (LEDx_GPIO).
 *
 * \note The pins of the specified LEDs are set to GPIO output mode.
 */
#define LED_Off(led_gpio)     port_pin_set_output_level(led_gpio, false)

/**
 * \brief Turns on the specified LEDs.
 *
 * \param led_gpio LED to turn on (LEDx_GPIO).
 *
 * \note The pins of the specified LEDs are set to GPIO output mode.
 */
#define LED_On(led_gpio)      port_pin_set_output_level(led_gpio, true)

/**
 * \brief Toggles the specified LEDs.
 *
 * \param led_gpio LED to toggle (LEDx_GPIO).
 *
 * \note The pins of the specified LEDs are set to GPIO output mode.
 */
#define LED_Toggle(led_gpio)  port_pin_toggle_output_level(led_gpio)



#endif // USER_BOARD_H

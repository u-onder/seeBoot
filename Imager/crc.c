//////////////////////////////////////////////////////////////////////////
//
//  FUNCTION NAME :
//      CRC_CalcBlockCRC
//
//
//  FUNCTIONAL DESCRIPTION :
//      Calculate a CRC the same way as the STM32F10x hardware generator.
//
//
//  FORMAL INPUT PARAMETERS :
//      buffer - pointer to series of 32-bit words (NOT bytes).
//      words  - count of 32-bit words to do.
//
//
//  FORMAL OUTPUT PARAMETERS :
//      None
//
//
//  RETURN VALUE :
//      32-bit CRC value as per the STM32F10x CRC generator.
//
//
//  IMPLICIT INPUTS AND OUTPUTS :
//      None
//
//
//  IMPORTED FUNCTIONS AND MACROS :
//      cm_ini(), cm_nxt(), cm_crc()
//
//
//////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "crcmodel.h"

unsigned int  CRC_CalcBlockCRC(unsigned int *buffer, unsigned int words)
{
cm_t			 crc_model;
unsigned int     word_to_do;
unsigned char    byte_to_do;
int			     i;

    // Values from the STM32F CRC generator.

    crc_model.cm_width = 32;            // 32-bit CRC
    crc_model.cm_poly  = 0x04C11DB7;    // CRC-32 polynomial
    crc_model.cm_init  = 0xFFFFFFFF;    // CRC initialized to 1's
    crc_model.cm_refin = FALSE;         // CRC calculated MSB first
    crc_model.cm_refot = FALSE;         // Final result is not bit-reversed
    crc_model.cm_xorot = 0x00000000;    // Final result XOR'ed with this

    cm_ini(&crc_model);

    while (words--)
    {
        // The STM32F10x hardware does 32-bit words at a time!!!

        word_to_do = *buffer++;

        // Do all bytes in the 32-bit word.

        for (i = 0; i < sizeof(word_to_do); i++)
        {
            // We calculate one *byte* at a time. If the CRC is MSB first we
            // do the next MS byte and vica-versa.

            if (crc_model.cm_refin == FALSE)
            {
                // MSB first. Do the next MS byte.

                byte_to_do = (unsigned char) ((word_to_do & 0xFF000000) >> 24);
                word_to_do <<= 8;
            }
            else
            {
                // LSB first. Do the next LS byte.

                byte_to_do = (unsigned char) (word_to_do & 0x000000FF);
                word_to_do >>= 8;
            }

            cm_nxt(&crc_model, byte_to_do);
        }
    }

    // Return the final result.

    return (cm_crc(&crc_model));
}

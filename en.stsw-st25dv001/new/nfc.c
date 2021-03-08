
/**
  * @brief  This function initializes the NFC Tag to perform the NDEF demos.
  * @details The tag is configured to its default and a CC file is written.
  */
void NDEF_DEMO_Init_Tag(void)
{
    /* Tag configuration for NDEF */
    BSP_LPD_WritePin(GPIO_PIN_RESET);
    PresentPasswd( true );

    /* Disable Mailbox mode */
    DeInitMailBoxMode();
    /* Activate RF */
    BSP_NFCTAG_WriteRFMngt(BSP_NFCTAG_INSTANCE, 0);
    
    /* Disable Energy Harvesting */
    BSP_NFCTAG_ResetEHENMode_Dyn(BSP_NFCTAG_INSTANCE);
  
    /* De-activate any protection */
    uint32_t mem_size = BSP_NFCTAG_GetByteSize(BSP_NFCTAG_INSTANCE);
    ST25DV_PROTECTION_ZONE ZoneRF;
    ST25DV_RF_PROT_ZONE no_rf_prot = {.PasswdCtrl = ST25DV_NOT_PROTECTED , .RWprotection = ST25DV_NO_PROT};
    for(ZoneRF = ST25DV_PROT_ZONE1; ZoneRF <= ST25DV_PROT_ZONE4 ; ZoneRF++)
    {
      ST25_RETRY(BSP_NFCTAG_WriteRFZxSS(BSP_NFCTAG_INSTANCE, ZoneRF, no_rf_prot));
      ST25_RETRY(BSP_NFCTAG_WriteI2CProtectZonex(BSP_NFCTAG_INSTANCE, ZoneRF, ST25DV_NO_PROT));
    }
    ST25_RETRY(BSP_NFCTAG_InitEndZone(BSP_NFCTAG_INSTANCE));

    ST25_RETRY(BSP_NFCTAG_WriteLockCCFile(BSP_NFCTAG_INSTANCE, ST25DV_CCFILE_2BLCK, ST25DV_UNLOCKED));
    
    NfcTag_SelectProtocol(NFCTAG_TYPE5);
    
    if(NfcType5_NDEFDetection() != NDEF_OK)
    {
      /* Write CC file content */
      CCFileStruct.MagicNumber  = NFCT5_MAGICNUMBER_E1_CCFILE;
      CCFileStruct.Version      = NFCT5_VERSION_V1_0;
      /* Size must be specified by block of 8 bytes */
      if(mem_size > (0xFF*8))
      {
        CCFileStruct.MagicNumber  = NFCT5_MAGICNUMBER_E2_CCFILE;
        CCFileStruct.MemorySize   = NFCT5_EXTENDED_CCFILE;
        CCFileStruct.ExtMemorySize   = (uint16_t)(mem_size / 8);
      
      } else {
        CCFileStruct.MemorySize   = (uint8_t)(mem_size / 8);
      }
      CCFileStruct.TT5Tag       =  NDEF_DEMO_CC3_COMPLIANT_VALUE;

      if(NfcType5_TT5Init() != NDEF_OK)
      {
        Menu_MsgStatus("No CCFILE","CCFILE init failed!",MSG_STATUS_ERROR);
      }
    }
 
}






/**
  * @brief  This function writes an URL to the Tag.
  */
void NDEF_DEMO_Write_URI_URL(void)
{

	uint16_t status;
	sURI_Info r_uri = {0};
  
  NDEF_DEMO_Init_Tag();

	/* Write URI for ST25 products web page */
	sURI_Info w_uri = {URI_ID_0x01_STRING, NDEF_DEMO_URL ,""};
	status = NDEF_WriteURI(&w_uri);

  /* Read back written URI */	
	if(status == NDEF_OK)
  {
    status = getURI(&r_uri);
	}
  
	if((status != NDEF_OK) || \
      strcmp(r_uri.protocol,w_uri.protocol) || \
      strcmp(r_uri.URI_Message,w_uri.URI_Message))
	{
		Menu_MsgStatus("Store URI failure!","URL has not been written!",MSG_STATUS_ERROR);
	}
  else
  {
    char msg[200] = "The URL has been written:\n";
    strcat(msg,URI_ID_0x01_STRING);
    strcat(msg,NDEF_DEMO_URL);
    strcat(msg,"\n\nPlease read it with a phone!");
		Menu_MsgStatus("Store URI success!",msg,MSG_STATUS_SUCCESS);
	}

}	
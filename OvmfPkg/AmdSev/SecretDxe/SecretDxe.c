/** @file
  Confidential Computing Secret configuration table constructor

  Copyright (C) 2020 James Bottomley, IBM Corporation.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <PiDxe.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Guid/ConfidentialComputingSecret.h>

STATIC CONFIDENTIAL_COMPUTING_SECRET_LOCATION mSecretDxeTable = {
  FixedPcdGet32 (PcdSevLaunchSecretBase),
  FixedPcdGet32 (PcdSevLaunchSecretSize),
};

STATIC EFI_EVENT mSecretDxeExitBootEvent;

/**
  ExitBootServices event notification function for the secret table.

  This function erases the content of the secret area so the secrets don't leak
  via released BootServices memory.  If the OS wants to keep the secrets for
  its own use, it must copy the secrets area to another memory area before
  calling ExitBootServices (for example in efi/libstub in Linux).

  @param[in] Event         The ExitBoot event that has been signaled.

  @param[in] Context       Unused.
**/
STATIC
VOID
EFIAPI
SecretDxeExitBoot (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  ASSERT(mSecretDxeTable.Base != 0);
  ASSERT(mSecretDxeTable.Size > 0);

  ZeroMem ((VOID *) ((UINTN) mSecretDxeTable.Base), mSecretDxeTable.Size);
}


EFI_STATUS
EFIAPI
InitializeSecretDxe(
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS Status;

  Status = gBS->InstallConfigurationTable (
                  &gConfidentialComputingSecretGuid,
                  &mSecretDxeTable
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return gBS->CreateEvent (EVT_SIGNAL_EXIT_BOOT_SERVICES, TPL_CALLBACK,
                SecretDxeExitBoot, NULL, &mSecretDxeExitBootEvent);
}

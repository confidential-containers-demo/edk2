/** @file
  Confidential Computing Secret configuration table constructor

  Copyright (C) 2020 James Bottomley, IBM Corporation.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h> // AllocateRuntimePool()
#include <Guid/ConfidentialComputingSecret.h>

EFI_STATUS
EFIAPI
InitializeSecretDxe (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  CONFIDENTIAL_COMPUTING_SECRET_LOCATION *SecretDxeTable;

  SecretDxeTable = AllocateRuntimePool (sizeof (CONFIDENTIAL_COMPUTING_SECRET_LOCATION));
  ASSERT (SecretDxeTable != NULL);

  SecretDxeTable->Base = FixedPcdGet32 (PcdSevLaunchSecretBase);
  SecretDxeTable->Size = FixedPcdGet32 (PcdSevLaunchSecretSize);

  return gBS->InstallConfigurationTable (
                &gConfidentialComputingSecretGuid,
                SecretDxeTable
                );
}

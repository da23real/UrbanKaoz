#pragma once
FLASHMEM void objectName07(uint8_t target){
 nameOwner07=selectedTrack;nameMod07=modSelected806;nameReturnMod07=modUi805;
 patchNameTarget808=target;patchNamePos808=0;++nameSession809;
 snprintf(patchNameDraft808,sizeof(patchNameDraft808),"%s",target==3?trackNames07[nameOwner07]:modNames07[nameMod07]);
 modUi805=0;patchUi808=4;patch808Render();
}
FLASHMEM void objectNameDone07(bool commit){if(patchNameTarget808==5){autoNameDone46(commit);return;}
 if(commit){char*dst=patchNameTarget808==3?trackNames07[nameOwner07]:modNames07[nameMod07];snprintf(dst,8,"%.7s",patchNameDraft808);storageDirty931=true;}
 patchUi808=0;modUi805=nameReturnMod07;nameDisplayR8();
 if(patchNameTarget808==3)sendTrackMenu();else {mod805Menu();displayValueDirty=true;}
}

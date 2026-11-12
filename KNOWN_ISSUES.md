# Known issues / Bugs connus

## English

- **Dense transitions.** Notes can be dropped when switching patterns under heavy load. A VA01.59 optimization reduced the worst tick from 327.77 to 165.68 ms in a 16-track test and increased delivered notes from 3,887 to 3,948 out of 4,096. This remains open.
- **SD detection.** The card was detected before the last VA01.60 flash and unavailable after reboot. The cause remains open.
- **Analog Rytm SMP and Roll-MOD.** Intermittent behavior remains open. Hardware Save/Reload and full-measure rolls under load still need validation.
- **WRITE.** Writing special Delay targets and ROL/DLY modulation triggers remains unimplemented.

## Français

- **Transitions denses.** Des notes peuvent manquer lors de changements de pattern sous forte charge. Une optimisation VA01.59 a réduit le pire temps de tick de 327,77 à 165,68 ms sur un banc à 16 pistes, et amélioré le nombre de notes reçues de 3 887 à 3 948 sur 4 096. Le problème reste ouvert.
- **Détection SD.** La carte était détectée avant le dernier flash VA01.60 et indisponible après le redémarrage. La cause reste à établir.
- **SMP Analog Rytm et Roll-MOD.** Les intermittences restent ouvertes. Le Save/Reload matériel et les rolls d’une mesure sous charge restent à valider.
- **WRITE.** L’écriture des cibles spéciales du Delay et des triggers MOD ROL/DLY reste à implémenter.

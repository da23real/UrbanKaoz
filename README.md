# UrbanKaoz Beta V02.00

[English](#english) · [Français](#français)

## English

### Why I built UrbanKaoz

I couldn’t find a sequencer that fit the way I make music. The MPC-style grooveboxes I knew had the groove I wanted for urban kiz, dancehall or even freetekno, but offered little of the MIDI/CV control I needed for envelopes, LFOs and other modulation. The MIDI/CV sequencers I tried felt too rigid for Afrobeat-style sequences. I wanted more flexible groove templates and an MPC-style Remix function. MIDI/CV delays and LFOs were often locked to tempo, and sidechain options were limited or missing.

To arrange a full track, I wanted a Song mode with a free data lane on each track. I could use it for a long MIDI CC automation or record a synth solo, then edit that material independently of the patterns.

I wanted the freedom to experiment with sidechain routing, and I accept that some complex combinations can still expose bugs. In my own use, UrbanKaoz is already very stable and fast for a DIY project, thanks in part to code optimizations I made with AI assistance.

[![Watch the English presentation on YouTube](https://i.ytimg.com/vi/zym04kyho6A/hqdefault.jpg)](https://www.youtube.com/watch?v=zym04kyho6A&t=1869s)

In this video I explain why I started building my own sequencer.

### The pre-roll, the star of my sequencer

This is the feature I’d been dreaming of for years and had never seen anywhere else. I can take a kick and decide how far **before** it the roll starts. If I move the kick, **the entire roll moves with its target note**. I can also put the roll after the note or spread it on both sides. [Hear the roll around 19:00](https://www.youtube.com/watch?v=zym04kyho6A&t=1140s).

I can stretch the roll, speed it up or slow it down along an exponential curve, and shape the velocity separately to build a crescendo. I can tweak it at any time, copy or save its settings, or write the generated notes directly into the sequence.

I can also choose whether roll notes trigger a delay or modulation. With a kick roll, for example, I can **keep the sidechain envelope trigger on the target kick** to duck the bass and **disable it for the roll notes**.

### Explore and adapt the project

Give your AI assistant the source for the version you’re using and describe what you want to hear. For example:

> I want a kick to trigger three repeats, each quieter and closer to the previous one. Can I do that with the existing delay settings? If so, walk me through it. If it takes a code change, point me to the relevant module, files and functions.

The [Teensy](Sources/UrbanKaoz_Tee_A01) runs the sequencer, the [Picran](Sources/UrbanKaoz_Picran_A01) drives the displays, and the [Pidi](Sources/Pidi_A01) helps handle MIDI/CV processing and output. Picran and Pidi are names I gave my modules to tell the Raspberry Pi Picos apart when working on the code with AI. Snkoz is the Windows companion app.

### I’d like to hear what you think

I originally built UrbanKaoz for my own music. I don’t yet know whether people would be interested in buying a finished version, bringing some of its features to another sequencer, or using it in a way I haven’t thought of. **Leave a comment telling me what you’d do with it or which feature you’d like to see elsewhere.** Your feedback will help me decide where to take the project next. [English video](https://www.youtube.com/watch?v=zym04kyho6A&t=1869s)

If you’d like to support development, you can [find me on Ko-fi](https://ko-fi.com/ers115).

[Source versions](SOURCE_VERSIONS.md) · [Known issues](KNOWN_ISSUES.md#english) · [MIT license](LICENSE)

## Français

### Pourquoi j’ai construit UrbanKaoz

Je ne trouvais pas chaussure à mon pied. Les séquenceurs de style MPC que je connaissais avaient les grooves que je cherchais pour l’urban kiz, le dancehall ou même la freetekno, mais offraient très peu de possibilités de contrôle MIDI/CV, notamment pour les enveloppes et les LFO. Les séquenceurs MIDI/CV que j’avais essayés étaient trop rigides pour des séquences de type afrobeat. Je cherchais des feuilles de groove plus souples et une fonction Remix dans l’esprit MPC. Les delays et les LFO MIDI/CV étaient souvent calés sur le tempo, et les possibilités de sidechain étaient limitées ou inexistantes.

Pour arranger un morceau entier, je cherchais un **mode Song avec une zone libre par piste**. Cette zone peut contenir une longue évolution de CC MIDI ou un solo de synthé, et s’éditer indépendamment des patterns.

J’ai voulu garder la liberté d’expérimenter avec le sidechain, et j’assume que certaines combinaisons complexes peuvent encore faire apparaître des bugs. Dans mon usage actuel, UrbanKaoz est déjà très stable et performant pour un projet DIY, grâce notamment aux optimisations de code réalisées avec l’aide de l’IA.

[![Voir la présentation française sur YouTube](https://i.ytimg.com/vi/oEWKTngDdS0/hqdefault.jpg)](https://www.youtube.com/watch?v=oEWKTngDdS0)

Dans cette vidéo, je raconte ce qui m’a donné envie de construire mon propre séquenceur.

### Le pré-roll, star de mon séquenceur

C’est la nouveauté dont je rêvais depuis longtemps et que je n’avais jamais vue ailleurs. Je peux partir d’un kick et décider à quel moment le roulement commence **avant** lui. Si je déplace le kick, **le roulement entier suit sa note cible**. Je peux aussi placer le roll après la note ou le répartir de part et d’autre.

Je peux étirer le roulement, le faire accélérer ou ralentir selon une courbe exponentielle, et régler séparément l’évolution de la vélocité pour créer un crescendo. Je peux reprendre ces réglages à tout moment, copier ou sauvegarder les paramètres d’un roll élaboré, ou écrire les notes générées directement dans la séquence.

Je peux aussi choisir si les notes du roll déclenchent un delay ou une modulation. Sur un roulement de kick, je peux **garder le trigger de l’enveloppe sidechain sur le kick cible** pour baisser le volume de la basse, et **le désactiver pour les notes du roulement**. [Voir la présentation du roll vers 13:00](https://www.youtube.com/watch?v=oEWKTngDdS0&t=780s)

### Explorer et adapter le projet

Donne à ton assistant IA les sources de la version que tu utilises et décris ce que tu veux entendre. Par exemple :

> Je veux que mon kick déclenche trois répétitions de plus en plus rapprochées et de moins en moins fortes. Est-ce possible avec les réglages du delay ? Si oui, guide-moi. Si ça demande une modification, indique-moi le module, les fichiers et les fonctions à examiner.

Le [Teensy](Sources/UrbanKaoz_Tee_A01) fait tourner le séquenceur, le [Picran](Sources/UrbanKaoz_Picran_A01) pilote les écrans et le [Pidi](Sources/Pidi_A01) participe au traitement et aux sorties MIDI/CV. Picran et Pidi sont les noms que j’ai donnés à mes modules pour distinguer les différents Raspberry Pi Pico quand je travaille sur le code avec l’IA. Snkoz est le logiciel Windows compagnon.

### Ton avis m’intéresse

J’ai d’abord construit UrbanKaoz pour ma propre musique. Je ne sais pas encore si des gens seraient intéressés par l’achat d’une version finalisée, par l’intégration de certaines fonctions à un autre séquenceur ou par un usage auquel je n’ai pas pensé. **Dis-moi en commentaire ce que tu en ferais ou quelle fonction tu aimerais retrouver ailleurs.** Tes retours m’aideront à décider de la suite. [Vidéo française](https://www.youtube.com/watch?v=oEWKTngDdS0)

Si tu veux soutenir le développement, tu peux [me retrouver sur Ko-fi](https://ko-fi.com/ers115).

[Versions des sources](SOURCE_VERSIONS.md) · [Bugs connus](KNOWN_ISSUES.md#français) · [Licence MIT](LICENSE)

# 🎹 GMMK2 ISO-DE Custom QMK Firmware

> **Eine persönliche Firmware-Adaption für die Glorious GMMK2 (96% und 65%) mit deutschem ISO-Layout.**
>
> Ich habe lange nach einer angenehm nutzbaren QMK-Firmware für diese Tastatur mit deutschem Sprachlayout (QWERTZ, Umlaute) gesucht und keine gefunden, die meinen Anforderungen entsprochen hätte. Also habe ich sie selbst gebaut – und hoffe, dass sie noch anderen deutschsprachigen GMMK2-Nutzern nützt.

---

## ⬇️ Download & Flash

### Firmware-Dateien

| Modell | Download |
|---|---|
| **GMMK2 96% ISO** | [`gmmk_gmmk2_p96_iso_iso_de_custom.bin`](firmware/gmmk_gmmk2_p96_iso_iso_de_custom.bin) |
| **GMMK2 65% ISO** | [`gmmk_gmmk2_p65_iso_iso_de_custom.bin`](firmware/gmmk_gmmk2_p65_iso_iso_de_custom.bin) |

> ⚠️ **Achtung:** Nur für die **ISO**-Varianten. Für ANSI-Layouts ist diese Firmware nicht geeignet.

### Flashen mit QMK Toolbox

1. **[QMK Toolbox herunterladen](https://github.com/qmk/qmk_toolbox/releases)** und starten
2. Oben rechts die passende `.bin`-Datei laden (**Open** → Datei auswählen)
3. Tastatur in den **Bootloader-Modus** versetzen – je nach Hardware-Revision gibt es zwei Methoden:

   **Methode A – ältere Revisionen (Tastatur bereits angesteckt):**
   - **FN + ESC** drücken
   - Alternativ: Reset-Taster auf der Rückseite der Tastatur drücken

   **Methode B – neuere Revisionen (Tastatur noch nicht angesteckt):**
   - **Leertaste + B** gleichzeitig gedrückt halten
   - Tastatur dabei per USB anstecken
   - Tasten erst loslassen, wenn QMK Toolbox die Verbindung meldet

   > ℹ️ Wenn **FN + ESC** keine Reaktion zeigt, ist Methode B die richtige Wahl.
   > Beide Methoden funktionieren bei P96 und P65 gleich.
   > Auch der offizielle [Glorious QMK Installation Guide](https://www.gloriousgaming.com/en-eu/blogs/guides-resources/gmmk-2-qmk-installation-guide) beschreibt **Space + B** als primäre Methode.

4. QMK Toolbox zeigt im Log: `WB32 DFU device connected`
5. **Flash** klicken – fertig!

> 💡 **Beim ersten Flash nach einem früheren VIA-Einsatz:** Halte beim Einstecken der Tastatur die **ESC-Taste gedrückt** – das löscht den EEPROM und stellt sicher, dass die neue Tastenbelegung korrekt übernommen wird.

---

## ✨ Features

### 🔧 Key Chatter Fix (Debounce = 25 ms)
Viele Schalter prellen bis zu 20 ms und verursachen dabei Doppeleingaben. Der QMK-Standard liegt bei nur 5 ms. Dieser Wert wurde auf **25 ms** erhöht, um auch stark prellende Schalter sicher abzufangen.

### 🇦🇹 Deutsches ISO-Layout (QWERTZ)
- Vollständige QWERTZ-Tastenbelegung für DE/AT
- Umlaute direkt auf den gewohnten Positionen: **Ö, Ä, Ü, ß**
- Sonderzeichen korrekt: **^, <, #, +, ´**
- Z und Y korrekt vertauscht gegenüber QWERTY

### 🎛️ VIA-Unterstützung
- VIA-Support ist aktiviert (`VIA_ENABLE = yes`)
- Tastenänderungen können jederzeit per Browser über **[usevia.app](https://usevia.app)** vorgenommen werden, ohne neu zu kompilieren
- 2 Keymap-Layer sind im EEPROM gespeichert

### 💡 Anpassbare Hintergrundbeleuchtung

#### Basisfarbe (10 Presets, persistent gespeichert)
Über **FN + →** und **FN + ←** wird zwischen 10 Basisfarben gewechselt. Die gewählte Farbe bleibt nach dem Ausschalten erhalten.

| # | Farbe | RGB |
|---|---|---|
| 0 | 🔵 Blau (Standard) | `0, 0, 255` |
| 1 | 🟡 Gelb | `255, 200, 0` |
| 2 | 🟣 Violett | `148, 0, 211` |
| 3 | 🌸 Rosa | `255, 20, 147` |
| 4 | 🩵 Türkis | `0, 200, 200` |
| 5 | 🟠 Orange | `255, 80, 0` |
| 6 | 🟢 Hellgrün | `50, 255, 50` |
| 7 | ⚪ Weiß | `255, 255, 255` |
| 8 | 🔴 Rot | `255, 0, 0` |
| 9 | ⚫ Aus | `0, 0, 0` *(Reaktiv-Effekte bleiben aktiv)* |

#### Helligkeit
**FN + ↑** und **FN + ↓** steuern die globale Helligkeit. Wird ebenfalls persistent gespeichert.

### 🌊 Reaktive RGB-Effekte

| Taste | Effekt |
|---|---|
| **ESC** | Roter Wellenring, der sich radial über die gesamte Tastatur ausbreitet |
| **ENTER** | Identischer roter Welleneffekt |
| **Alle anderen Tasten** | Kurzer grüner Flash auf der gedrückten Taste (500 ms Fade-Out) |

> Die Reaktiv-Effekte sind *immer* aktiv – auch im Basisfarben-Modus „Aus" (Index 9).

### 🔢 Num Lock Indikator *(nur P96)*
- Num Lock **AN** → Die Num-Lock-Taste leuchtet in der gewählten Basisfarbe
- Num Lock **AUS** → Die Num-Lock-Taste ist dunkel

---

## 🔄 Original-Firmware wiederherstellen

Auf der offiziellen Seite von Glorious findest du die originale Firmware sowie eine Installationsanleitung, mit der du deine Tastatur wieder in den Auslieferungszustand versetzen kannst:

👉 **[GMMK 2 QMK Installation Guide – Glorious](https://www.gloriousgaming.com/en-eu/blogs/guides-resources/gmmk-2-qmk-installation-guide)**

---

## ⌨️ Vollständige Tastenbelegung

### GMMK2 96% ISO

#### Layer 0 – Basis (QWERTZ DE/AT)

```
┌──────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬──────┬────┬────┬────┬────┐
│  ESC │ F1 │ F2 │ F3 │ F4 │ F5 │ F6 │ F7 │ F8 │ F9 │F10 │F11 │F12 │PrtSc │Del │Ins │PgUp│PgDn│
├──────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼──────┼────┼────┼────┼────┤
│  ^   │ 1  │ 2  │ 3  │ 4  │ 5  │ 6  │ 7  │ 8  │ 9  │ 0  │ ß  │ ´  │ Bksp │NmLk│ /  │ *  │ -  │
├──────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼──────┼────┼────┼────┼────┤
│ Tab  │ Q  │ W  │ E  │ R  │ T  │ Z  │ U  │ I  │ O  │ P  │ Ü  │ +  │      │ 7  │ 8  │ 9  │    │
├──────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┤Enter ├────┼────┼────┤ +  │
│ Caps │ A  │ S  │ D  │ F  │ G  │ H  │ J  │ K  │ L  │ Ö  │ Ä  │ #  │      │ 4  │ 5  │ 6  │    │
├──────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼──────┼────┼────┼────┼────┤
│LShift│ <  │ Y  │ X  │ C  │ V  │ B  │ N  │ M  │ ,  │ .  │ -  │RShft│  ↑  │ 1  │ 2  │ 3  │Ent │
├──────┼────┼────┼─────────────────────────────────────┼────┼────┼────┼────┼────┼────┼────┼────┤
│LCtrl │Win │LAlt│            Space                    │RAlt│ FN │RCtrl│ ← │ ↓  │ →  │ 0  │ .  │
└──────┴────┴────┴─────────────────────────────────────┴────┴────┴────┴────┴────┴────┴────┴────┘
```

#### Layer 1 – FN (gehalten)

```
┌──────┬──────┬────────┬──────┬───────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬────┬──────┬────┬────┬──────┬─────┐
│ BOOT │MyCmp │Browser │ Calc │MediaSl│Prev  │ RW   │ Play │ Stop │ Mute │Vol + │Vol - │ —  │  —   │ —  │ —  │ Home │ End │
├──────┼──────┼────────┼──────┼───────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼────┼──────┼────┼────┼──────┼─────┤
│  —   │  —   │   —    │  —   │   —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │ —  │  —   │ —  │ —  │  —   │  —  │
├──────┼──────┼────────┼──────┼───────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼────┼──────┼────┼────┼──────┼─────┤
│  —   │  —   │   —    │  —   │   —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │ —  │      │ —  │ —  │  —   │     │
├──────┼──────┼────────┼──────┼───────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼────┤  —   ├────┼────┼──────┤  —  │
│  —   │  —   │   —    │  —   │   —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │ —  │      │ —  │ —  │  —   │     │
├──────┼──────┼────────┼──────┼───────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┼────┼──────┼────┼────┼──────┼─────┤
│  —   │  —   │   —    │  —   │   —   │  —   │  —   │  —   │  —   │  —   │  —   │  —   │ —  │Brit+ │ —  │ —  │  —   │  —  │
├──────┼──────┼────────┼──────────────────────────────────────────┼──────┼────┼────┼──────┼──────┼──────┼────┼─────┤
│  —   │  —   │   —    │                   —                      │  —   │ —  │ —  │Prev C│Brit-│Next C│ —  │  —  │
└──────┴──────┴────────┴──────────────────────────────────────────┴──────┴────┴────┴──────┴──────┴──────┴────┴─────┘

Legende: — = transparent (Taste aus Layer 0 wird verwendet)
         Brit+/- = Helligkeit +/–  |  Prev/Next C = Basisfarbe wechseln
         BOOT = Bootloader-Modus
```

---

### GMMK2 65% ISO

#### Layer 0 – Basis (QWERTZ DE/AT)

```
┌──────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬──────┬────┐
│  ESC │ 1  │ 2  │ 3  │ 4  │ 5  │ 6  │ 7  │ 8  │ 9  │ 0  │ ß  │ ´  │ Bksp │Del │
├──────┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┐     ├────┤
│  Tab  │ Q  │ W  │ E  │ R  │ T  │ Z  │ U  │ I  │ O  │ P  │ Ü  │ +  │     │PgUp│
├───────┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┤Ent ├────┤
│  Caps  │ A  │ S  │ D  │ F  │ G  │ H  │ J  │ K  │ L  │ Ö  │ Ä  │ #  │    │PgDn│
├────────┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┴┬───┤
│ LShift  │ <  │ Y  │ X  │ C  │ V  │ B  │ N  │ M  │ ,  │ .  │ -  │RSft│ ↑  │End│
├──────┬──┴─┬──┴──┬─┴────┴────┴────┴────┴────┴──┬──┴─┬──┴──┬─┴──┬─┴──┬─┴──┬─┴──┤
│LCtrl │Win │LAlt │            Space             │RAlt│ FN  │ ←  │ ↓  │ →  │    │
└──────┴────┴─────┴──────────────────────────────┴────┴─────┴────┴────┴────┴────┘
```

#### Layer 1 – FN (gehalten)

```
┌──────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬──────┬────┐
│ BOOT │ F1 │ F2 │ F3 │ F4 │ F5 │ F6 │ F7 │ F8 │ F9 │F10 │F11 │F12 │  —   │Ins │
├───────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┐      ├────┤
│   —   │ —  │ —  │ —  │ —  │ —  │ —  │ —  │ —  │ —  │ —  │ —  │ —  │  —   │Home│
├────────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┤  —  ├────┤
│   —    │ —  │ —  │ —  │ —  │ —  │ —  │ —  │ —  │ —  │ —  │ —  │ —  │     │ End│
├─────────┬────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬────┬────┬────┬───┬────┬────┤
│    —    │ —  │Prev │Stop │Play │Next │Mute │Vol+ │Vol- │ —  │ —  │ —  │ — │Brit+│ — │
├──────┬───┬──────┬──────────────────────────────────┬────┬──────┬──────┬──────┬──────┤
│  —   │ — │  —   │              —                   │ —  │  —   │PrevC │Brit- │NextC │
└──────┴───┴──────┴──────────────────────────────────┴────┴──────┴──────┴──────┴──────┘

Legende: — = transparent  |  Brit+/- = Helligkeit +/–  |  Prev/Next C = Basisfarbe
         BOOT = Bootloader  |  Prev/Stop/Play/Next = Medientasten
```

---

## 📋 FN-Tasten Kurzreferenz

### Gilt für beide Modelle

| FN + | Funktion |
|---|---|
| `↑` | 🔆 Helligkeit + |
| `↓` | 🔅 Helligkeit – |
| `→` | ➡️ Nächste Basisfarbe |
| `←` | ⬅️ Vorherige Basisfarbe |
| `ESC` | 🔧 Bootloader-Modus (zum Flashen) |
| `PgUp` | ⬆️ Pos1 / Home |
| `PgDn` | ⬇️ Ende / End |

### Nur P96

| FN + | Funktion |
|---|---|
| `F1` | 💻 Dateimanager |
| `F2` | 🌐 Browser |
| `F3` | 🧮 Taschenrechner |
| `F4` | 🎵 Mediathek |
| `F5` | ⏮ Vorheriger Titel |
| `F6` | ⏪ Rewind |
| `F7` | ⏯ Play / Pause |
| `F8` | ⏹ Stop |
| `F9` | 🔇 Mute |
| `F10` | 🔊 Lautstärke + |
| `F11` | 🔉 Lautstärke – |
| `Del` | Ins |

### Nur P65

| FN + | Funktion |
|---|---|
| `1`…`=` | F1 – F12 |
| `Del` | Ins |
| `Y` | ⏮ Vorheriger Titel |
| `X` | ⏹ Stop |
| `C` | ⏯ Play / Pause |
| `V` | ⏭ Nächster Titel |
| `B` | 🔇 Mute |
| `N` | 🔊 Lautstärke + |
| `M` | 🔉 Lautstärke – |

---

## 🛠️ Selbst kompilieren

Die Keymap-Quelldateien liegen in den Ordnern [`keymap_p96/`](keymap_p96/) und [`keymap_p65/`](keymap_p65/).

Als Basis-Repository wird der Fork von GloriousThrall benötigt, da nur dieser die korrekten RGB-Matrix-Treiber für den AW20216S-LED-Controller enthält:

👉 **[GloriousThrall/qmk_firmware](https://github.com/GloriousThrall/qmk_firmware/tree/master/keyboards/gmmk/gmmk2)**

**Kurzanleitung:**

```bash
# 1. Repository klonen
git clone https://github.com/GloriousThrall/qmk_firmware.git
cd qmk_firmware
git submodule update --init --recursive lib/lufa

# 2. Keymap-Ordner kopieren
#    Für P96:
cp -r /pfad/zu/keymap_p96  keyboards/gmmk/gmmk2/p96/iso/keymaps/iso_de_custom
#    Für P65:
cp -r /pfad/zu/keymap_p65  keyboards/gmmk/gmmk2/p65/iso/keymaps/iso_de_custom

# 3. Kompilieren
qmk compile -kb gmmk/gmmk2/p96/iso -km iso_de_custom
qmk compile -kb gmmk/gmmk2/p65/iso -km iso_de_custom
```

---

## 📄 Lizenz

Diese Keymap-Dateien stehen unter der **GNU General Public License v2** (GPL v2), entsprechend der Lizenz des QMK-Firmware-Projekts.

Das vollständige Basis-Framework (QMK) ist unter [qmk/qmk_firmware](https://github.com/qmk/qmk_firmware) verfügbar.

> *Die kompilierten `.bin`-Dateien enthalten QMK-Framework-Code, der ebenfalls unter GPL v2 steht. Der vollständige Quellcode dieser Anpassung ist in den Ordnern `keymap_p96/` und `keymap_p65/` dieses Repositories enthalten.*

---

## 🙋 Feedback & Beiträge

Wenn du Fehler findest, andere Layouts brauchst oder Verbesserungsvorschläge hast, freue ich mich über Issues oder Pull Requests!

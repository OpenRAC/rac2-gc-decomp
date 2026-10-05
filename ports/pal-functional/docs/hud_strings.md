## Mapping of Interface Text Strings (Ammo HUD)

These strings are used by the graphics engine to load visual assets or ammunition counter labels onto the HUD.

| Virtual Address (EE RAM) | String Value | Estimated Purpose |
|----------------------------|--------------|--------------------|
| `0x001ae6d8`               | “BackAmmo”   | Ammo HUD background |
| `0x001ae6e8`               | “OutlineAmmo”| Ammo outline/border |
| `0x001ae6f8`               | “WeaXP”      | Active weapon experience bar |
| `0x001ae700`               | “AmmoText”   | Font/Numeric text displaying the quantity |
| `0x001ae710`               | “AmmoIcon”   | Icon of the currently selected weapon |

## Mapping of Interface Text Strings (Bolt HUD) - Verified!

These strings configure the visual container that displays the total number of Bolts collected by the player.

| Virtual Address (EE RAM) | String Value | Confirmed Purpose |
|----------------------------|--------------|----------------------|
| `0x001ae720`               | “BackBolt”   | Visual background of the Bolt marker |
| `0x001ae730`               | “OutlineBolt”| Outline/outer border of the Bolt counter |
| `0x001ae740`               | “BoltText”   | Numeric text of the Bolt counter |

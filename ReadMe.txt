---------------------------[ Color Cop v5.5.1 ]--------------------------

Color Cop is a multi-purpose color picker for web designers and
programmers. It features an eyedropper, magnifier, variable magnification
levels, 3 by 3 and 5 by 5 average sampling, snap to websafe, color history,
and a complementary color palette.

Color Cop will convert RGB decimal values to Hexadecimal color codes.
It has color code support for HTML Hex, Delphi Hex, PowerBuilder,
Visual Basic Hex, Visual C++ Hex, RGB int, and RGB float.

-----------------------------------------------------------------------

Color Cop Website: https://colorcop.net

Color Cop is free, but if you wish to express your appreciation for
the time and resources I've expended developing and supporting it,
I do accept and appreciate donations. If you wish to make one please go to:
https://colorcop.net/donate



-------------- What's new in v5.4 --------------

* Magnify while eyedropping

  This option is on by default and allows the user to see a magnified area
  while eyedropping. The pixel that is being selected will appear in the
  magnifier window with a black square around it.

* Use Cross Hair cursor

  the standard windows cross hair cursor can be used in lieu of the
  eyedropper to select colors.

* Relative position measurements

  If you press the Control key while eyedropping the coordinates will be set
  to zero (0,0). When you drag the mouse to a new location the coordinates
  values will be relative to the point where you pressed control. This is
  very useful if you want to measure how pixels a certain object is. Note
  that if you get a reading of 8,8 the area you just measured
  is really 9,9 because the zero row and column are included.

* Detect WebSafe color

  With this option enabled, color cop will indicate if the color selected
  by the eyedropper is WebSafe or not.

* Allow Multiple Instances

  This is an option which allows the user to restrict only one instance
  of color cop from running at once, or allow multiple copies to run.





*** Color Cop Features ***

- Convert RGB Decimal values to HTML Hexadecimal
- Custom Color Picker
- Eyedropper
- Always on Top
- Persistent settings
- Convert Hexadecimal to RGB Decimal values
- Random Color Generator
- Reverse Color Option
- Magnifier with 1x-16x zoom
- 3 by 3 and 5 by 5 average sampling
- Snap to WebSafe
- Detect WebSafe color
- Minimize to systray
- HTML, Delphi, Visual Basic, Visual C++ and Powerbuilder Hex color support
- Mouse wheel support
- EasyMove
- Upper and Lower case Hex
- Collapseable dialog
- Color History
- Complementary Color Palette
- Relative position measurements


* Convert RGB Decimal values to HTML Hexadecimal

  Depending on the Mode, the Red, Green, and Blue values are converted to
  the hex color code.  These values can be typed in manually or selected
  with the eyedropper.  If the user spins the mouse wheel while the focus
  is on one of the controls it will increment or decrement depending on
  the direction.
  Holding CONTROL will +/- by 5. Holding SHIFT will +/- by 2.

* Custom Color Picker

  This is the windows common color picker dialog.  It can be used to slightly
  alter the current color.  Double-clicking on the color preview window will
  bring up this dialog.  The 16 custom color slots in the dialog are
  remembered from session to session.

* Eyedropper

  This control can be dragged around the screen to select any color.  If you
  want to ease the selection, use the Magnifier first.

* Always on Top

  When this option is checked, the program will stay on top of other windows.
  This allows Color Cop to be used in any window.

* Persistent Settings

  This feature saves your settings to ColorCop.dat.  These settings are
  reloaded the next time you start the application.  If you delete
  ColorCop.dat the settings will be lost.  Color Cop will remember
  screen position, last color, color history, mode, sampling rate, etc.

* Convert Hexadecimal to RGB Decimal values

  If Color Cop is in HTML or Delphi mode. Hex codes can be pasted/typed in
  and the the Color and RGB values will be given.  This is not available for
  Visual Basic, Visual C++ or PowerBuilder modes.

* Random Color Generator

  Generates a Random color.  Control+Z is the Hotkey.

* Reverse Color Option

  Selecting this option will reverse the current color.
  Black will become white, etc.

* Magnifier with 1x-16x zoom

  For areas of the screen with small icons or lots of colors, the Magnifier
  can be used to enlarge the area and then select colors.  The magnification
  level can be changed by spinning the mouse wheel while magnifying or by
  clicking on the plus and minus buttons which are located to the right of
  the magnifier

* 3 by 3 and 5 by 5 average sampling

  This feature allows the eyedropper to select an average of a 3 by 3 or
  a 5 by 5 pixel matrix.

* Snap to WebSafe

  Some designers are forced to use the 216 color websafe palette. When this
  options is enabled, only websafe colors can be selected. If a color is
  selected and it is not websafe, it will be converted to the nearest
  websafe color. Websafe colors have RGB decimal values which are multiples
  of 51 or zero.(0, 51, 102, 153, 204, and 255)

* Minimize to systray

  When this option is checked, Color cop will be minimized to the
  system tray rather than the taskbar.

* HTML, Delphi, Visual Basic, Visual C++, Clarion and Powerbuilder Hex color support

  There are now many Hex modes to choose from.  Programmers can now use
  Color Cop.  If you would like another mode added, email
  support@prall.net

* Mouse wheel support

  The mouse wheel can be used to increment or decrement the Red, Green, or
  Blue values.  It will also allow the user to zoom while magnifying.

* Arrow key support while magnifying or eyedropping

* EasyMove

  This option allows the user to move the window by left clicking and dragging
  anywhere in the Color Cop window.

* Upper and Lower case Hex

  Some people prefer their hex in uppercase and some in lowercase.

* Collapsible dialog

  This feature allows the application to compact itself to allow more
  screen area to select colors from.

* Color History

  This feature remembers the last 7 colors selected.  Colors can be pushed
  into the color history by right clicking while eyedropping.

* Complementary Color Palette

  The color palette uses the HSL color model to generate 42 colors that
  complement the selected color.  Colors in this pallete can be selected
  by left clicking.


*** Color Cop FAQ ***

  - How are these HTML RGB Hexadecimal Triplets used?

    Here is an HTML example.  The other programming hex codes should
    speak for themselves.

    <html><title>This is a sample</title>
    <body bgcolor="#ffffff">This makes the background color WHITE.
    <font color="#ffff00">This is YELLOW text.</font>
    <table width="100%" bgcolor="#0000ff">
    <tr>
    <td>This Background is Blue!!</td>
    </tr></table>
    </body></html>


  - How do I change options??  I don't see a menu?

    Right Click in the color cop window to get a pop up menu.



  For other problems please use Color Cop Help. (press F1 while in Color Cop)


** Requirements ***
- 24 Bit color
- A PC running Windows



*** Color Cop Revision History ***

v5.5.1 - 12/01/25 - Releasing from Github repository
                  - Switch to HTML-based help system for improved compatibility and accessibility
                  - Press F1 to open the new HTML help interface

v5.4.5 - 10/24/07 - added Clarion Hex mode
	                - added arrow key support while eyedropping or magnifying
	                - added multi-pixel average sampling 3x3 - 31x31
	                - started CMYK

v5.4.2 - 09/19/06 - added Put cursor over Eyedropper on startup feature
                  - fixed minimize on startup so that it doesn't flash when starting
                  - added digital signature

v5.4.1 - 09/15/06 - settings files are now stored in the Application Data directory

v5.4  - 09/03/06 - new Icon created by Raul Matei.
                 - changed location of .dat file and .bmp file
                 - updated the domain used in the urls

v5.3  - 02/23/03 - Added Use Cross Hair Cursor, Detect WebSafe color,
                 - Relative position measurements, magnify while eyedropping,
                 - Allow multiple instances, RGB int mode, and RGB float mode

v5.2   - 11/23/02 : Added Convert to Grayscale and the ability to
                    click on the magnified area and get that color.

v5.1.3 - 02/16/02 : Added Persistent magnifier bitmap
                    Shift F10 brings up the options menu
		    added options menu to the system menu for keyboard access

v5.0.1 - 07/28/01 : Added - Window Help File (Press F1)
                          - Fixed Plus and Minus button sensitivity
                          - Fixed About Box static text problem


v5.0 - 07/22/01 : Added - Magnifier with 1x-16x zoom
                        - 3 by 3 and 5 by 5 average sampling
                        - Snap to WebSafe
                        - Minimize to systray
                        - Visual Basic, Visual C++ and
                          Powerbuilder color code support
                        - Mouse wheel support
                        - EasyMove
                        - Upper and Lower case Hex
                        - Collapseable dialog
                        - Color History
                        - Complementary Color Palette

v4.0 - 05/17/00 : Added - dropdown menus
                        - Support for Delphi Hex codes
                        - Reverse Color Option
                        - Random Color Option
                        - Toggleable Always on Top
                : fixed minimize bug. (if you closed older versions
                  while minimized, the next time you ran it, it would be stuck)

v3.6 - 02/20/00 : Color Cop can now convert from HEX to RGB
                : Added link to colorcop website in ABOUT
                : Persistent Settings
v3.5 - 11/11/99 : Added Eyedropper support
		: made dialog A LOT smaller
		: Added Always on top.(for use with eyedropper)
		: Added Remember Settings for next run - 32KB
v3.0 - 10/1/99  : Changed Fonts back to MS San Serif
		: Fixed the over 255 Bug
		: Enlarged Edit Boxes for oversized fonts - 28KB
v2.2 - 9/24/99  : Added Auto Copy to Clipboard - 28KB
v2.0 - 8/22/99  : Added Color Preview - 14KB
v1.0 - 8/15/99  : Added Custom Color Picker - 13KB

Still need help? or you have a suggestion? Email support@prall.net

--------------------------------------[ End of File ]------------------

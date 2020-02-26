(module# COLORTBL)

;;
;;    COLORTBL.SC
;;    (c) Sierra On-Line, Inc, 1996
;;
;;    Author(s): K. Sergent
;;
;;    Description: Allows use of color remapping for non human NPC's.
;;
;;    Usage:	In stkobj file where creature is defined, assign properties:
;;
;;					pClutStart: -1
;;					pColor: #  (to the table entry you wish to use)
;;
;;		Make sure that the color table you select applies to the creature
;;		you are modifying.
;;
;;    Examples:
;;
;;		(instance SOBJImp of Code
;;			(method (doit aWhatObj)
;;				(aWhatObj
;;					name: "Imp",
;;					pName: "Imp",
;;					pBaseView: 40400,
;;					pAction: 1,
;;					pClutStart: -1,
;;					pColor: 6, ;;color: 6 from table
;;				)
;;			)
;;		)
;;
;;		Color table AND scale!
;;
;;		formulae for size/color
;;		
;;		1. convert decimal color table entry to hex
;;		2. convert decimal size table entry to hex
;;		3. put values together (i.e. sizecolor)
;;		4. convert to decimal
;;
;;		example:
;;		color = 48
;;		size = 7
;;		48 hex = 30
;;		7 hex = 7
;;		together = 730
;;		730 decimal is 1840
;;
;;		(instance SOBJImp of Code
;;			(method (doit aWhatObj)
;;				(aWhatObj
;;					name: "Giant Imp",
;;					pName: "Giant Imp",
;;					pBaseView: 40400,
;;					pAction: 1,
;;					pClutStart: -1,
;;					pColor: $0307, ;color: 7 scale set: 3
;;				)
;;			)
;;		)
;;
;;    Code Map:
;;
;;        Procedures:
;;				SetClut	- updates the NPC's clut to new values
;;
;;        Instances:
;;
;;    History:
;;          9/13/96 Kerry Sergent
;;          Last Modified
;;
;;---------------------------------------------------------------------------

(include "wobject.sh")

;; The table contains 39 pair sets. With the format being:
;; (first = color to change from <space> second = color to change to)
;; 24 integers max. The table is read until -1 or 24 ints has been reached.

(public
	SetClut 0
)

(procedure
	SetClut
)

(define NUM_COLOR_ENTRIES 127) ;;set this to the value of the index terminator.
(define NUM_SCALE_ENTRIES 20)

(local

;_______________________
;
;	color table
;_______________________
;
	lRemapTable = [
;_______________________
;
;	Black wolf or rat
;_______________________
;
	 99   0  100  38  101   0  -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 ;;	0 black
	 99  88  100  89  101  90  -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 ;;	1 Flame Wolf
	 99 101  100 102  101 103  -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 ;;	2 White Wolf
	 99  58  100  59  101  60  -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 ;;	3 hellhound
;_______________________
;
;	humanoid colors
;_______________________
;
	104  88 105  89 106  90 107  91 88   0 89   0 90   0 90  0  -1 -1 -1 -1 -1 -1 -1 -1 ;; 4 orange
	104  58 105  59 106  60 107  61 88   0 89   0 90   0 90  0  -1 -1 -1 -1 -1 -1 -1 -1 ;; 5 red
	104  54 105  55 106  56 107  57 88  82 89  82 90  81 91  81 -1 -1 -1 -1 -1 -1 -1 -1 ;; 6 frost
	104 240 105 241 106 242 107 243 88 240 89 241 90 242 91 243 43 58 15 59 16 59 -1 -1	;; 7 wraith
;_______________________
;
;	wasp colors
;_______________________
;
	55 85 57 86 70 79  71 81 72 82 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 ;; 8 hive warrior
	65 50 66 51 91 49 103 52 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 ;; 9 swamp wasp
	55 59 57 61 -1 -1  -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 ;; 10 hive mage
	91 74 65 75 66 76  55 73 70 84 71 85 72 86 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 ;; 11	Mud Wasp

;_______________________
;
;	imp colors
;_______________________
;
	58  53  59  54  60  55  61  56  99  78 100  79 101  80  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 12 Water Imp
	58  73  59  74  60  58  61  57  99  98 100  99 101 100  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 13 Blood Imp
	99  64 100  65 101  66  58  99  59 100  60 101  61 102  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 14 Repentent Imp
	58  63  59  64  60  68  61  67  99  89 100  90 101  91  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 15 Sun Imp
	58 101  59 100  60  99  61  98  65 100  75 101  76 102 107 101 108 103  99  99 100 100 101 101 ;; 16 dark imp ??
	58  98  59  73  60  83  61  84  99 104 100 105 101 106  65  87  -1  -1  -1  -1  -1  -1  -1  -1 ;; 17 forest imp
	58  98  59  48  60  49  61  50  99  98 100  63 101  64  65  66  -1  -1  -1  -1  -1  -1  -1  -1 ;; 18 swamp imp
	99  73 100  74 101  75  58 104  59 105  60 106  61 107  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 19 gargoyle
	58  93  59  94  60  95  61  98  65  96  75  99  76 100 107 101 108 102 109 103 100 100 101 101 ;; 20 king imp ??
;;mike-fixed 21 - flame daemon, imp warrior
        58 101  59 107  60  75  61  98  65  60 100  59 101 108 107 101 108 102 109 103  75  99  76 100 ;; 21 fire imp
        58  80  59  81  60  82  61 103  99  80 100  81 101  82 108 103  65  79  -1  -1  -1  -1  -1  -1 ;; 22 ice imp
        58  98  59  99  60 100  61 101  75 100 107 101 108 102 109 102  65  60  -1  -1  -1  -1  -1  -1 ;; 23 Grey Gargoyle

;_______________________
;
;	troll colors
;_______________________
;

	22  61  66  60 107  59 106  58 -1  -1 -1  -1  -1  -1 -1  -1  -1  -1  -1  -1 -1 -1 -1 -1 ;; 24 troll warrior
	22  56  66  55 107  54 106  53 -1  -1 -1  -1  -1  -1 -1  -1  -1  -1  -1  -1 -1 -1 -1 -1 ;; 25 troll mage
;mike-fixed 26 rock troll 
;;	65 106  75 104  76 105  91 106 90 105 -1  -1  -1  -1 -1  -1  -1  -1  -1  -1 -1 -1 -1 -1 ;; 26 rock troll
       106  90 105  -1 104  76  65 106 75 104 76 105  90 105 91 106  -1  -1  -1  -1 -1 -1 -1 -1 ;; 26 rock troll (fuloran)
	65 101  74  98  75 100  76 101 87  61 90  98  91  99 99  99 100 100 101 102 -1 -1 -1 -1 ;; 27 demon troll ??

;_______________________
;
;	Banshee colors
;_______________________
;

  103  85 56  83  57  84  61  87 62  86 -1 -1 -1 -1 -1 -1  -1  -1  -1  -1 -1 -1 -1 -1 ;; 28 Jade Lady
  103  98 56 100  57  99  61  65 62  64 -1 -1 -1 -1 -1 -1  -1  -1  -1  -1 -1 -1 -1 -1 ;; 29 Dark Lady
  103  60 56  58  57  59  61  65 62  66 -1 -1 -1 -1 -1 -1  -1  -1  -1  -1 -1 -1 -1 -1 ;; 30 Fury
   56  51 57  52  61  85  62  86 -1  -1 -1 -1 -1 -1 -1 -1  -1  -1  -1  -1 -1 -1 -1 -1 ;; 31 Bog Banshee
  103  50 56  48  57  49  61  59 62  60 -1 -1 -1 -1 -1 -1  -1  -1  -1  -1 -1 -1 -1 -1 ;; 32 Rusalka
  103 100 56  99  57 100  61  89 62  88 -1 -1 -1 -1 -1 -1  -1  -1  -1  -1 -1 -1 -1 -1 ;; 33 Revenant

;_______________________
;
;	Skeleton colors
;_______________________
;
  101  98 100  98 102  99 110 100 109  61 108  60 63  58 64 59 49 53  50  54 51 55 52 56 ;; 34 Blackskull
  101  58 100  59 102  60 110  61 109 102 108 101 63 100 64 99 49 79  -1  -1 -1 -1 -1 -1 ;; 35 Bloodbones
  109 102 108 101  63  98  64  99  49  79  50  80 51  81 52 82 -1 -1  -1  -1 -1 -1 -1 -1 ;; 36 Skeleton Mage
  109  74 108  75  63  76  64  77  49  89  50  90 51  91 52 92 -1 -1  -1  -1 -1 -1 -1 -1 ;; 37 Greater Skeleton
  100  99 101 100 102 101 110 102 109  56 108  55 63  53 64 54 49 58  50  59 51 60 52 61 ;; 38 Skeleton Hero

; ________________
;
; new stuff
;
; ________________

  108  83 109  84  64  74  63  73  51  83  52  84  50  74  49  73 102  85 101  84 110  86 100  83 ;; 39 sea skeleton
   99   0 100  98 107 101 101  99  27   0  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 40 black rat
  100  74  99  73 101  75  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 41 brown rat
   66  85  65  84  91  83 103  86  55  86  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 42 worker wasp
   66  60  65  59  91  58 103  61  99  98  55  90  72  61  71  60  70  59  95  58  -1  -1  -1  -1 ;; 43 warrior wasp
   66  96  65  95  91  94 103  94  99  98  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 44 queen wasp
   64  60  65  58  66  60  67  61  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 45 seraph warrior
   65  56  64  86  66  81  67  55  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 46 seraph thaumaturgist
   65  71  64  68  66  99  67 100  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 47 seraph queen

;_______________________
;
;	Cobra colors
;_______________________
;
	48  58  49  59  50  60  51  61  52  88  98  89 -1  -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 ;;	48 Hell Snake/Lava Serpent (red)
	48  98  49  99  50 100  51 101  52 102  98  41 -1  -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 ;;	49 Baby Dragon (black/grey)

;_______________________
;
;	Elemental colors
;_______________________
;
	98 240  99 241 100 242 101 243 102  59  -1  -1 -1  -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 ;;	50 Air Elemental (semi transparent)

;_______________________
;
;	Gargoyle Colors
;_______________________
;

  78 68 79 69 80 70 81 71 85 59 101 96 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1	;;	51 -- pansy gargoyle


;; MIKE - this block was put here because some weird things call for these remaps, even though they
;; never existed before.
;; Fuloran and Wraiths use remap 53
;; Somewhere in the code, clutStart might be getting set to -1, which
;; might explain the weird color requests coming through here.
;; Gotta track it down later.

   -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 52
   -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 53

  
;_______________________
;
;	Minotaur Colors
;_______________________
;
  109 108 108 107 107 106 106 105  63 100  64 101  65 102  58  49  59  50  60  51  61  52  84  67 ;; 54 Thrall Minotaur
  109 110 108 109 107 108 106 107  63 101  64 102  65 103  58  73  59  74  60  75  61  76  84  70 ;; 55 Drudge Minotaur
   65  90  64  89  63  88  61  86  60  85  59  84  58  83  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 56 Invader Minotaur
   63  75  64  76  65  77  84  55  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 57 Warrior Minotaur
  109 107 108 106 107 105 106 104  63  98  64  99  65 100  58  99  59  83  60  84  61  85  84  90 ;; 58 Berserker Minotaur

;; Minotaur Mages
   63 101  64 102  65 103  58  68  59  69  60  70  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 59 Acolyte
   63  75  64  76  65  77  58  63  59  64  60  65  53 101  54 102  55 103  84  53  85  54  86  55 ;; 60 Cleric
   63  73  64  74  65  75  58  48  59  49  60  50  84  55  85  56  86  57  53  99  54  83  55  84 ;; 61 Druid
   63  99  64 100  65 101  58  89  59  90  60  91  53  93  54  94  55  95  84  79  85  80  86  81 ;; 62 Wizard
   63  98  64  99  65 100  53  98  54  99  55 100  84  89  85  90  86  91  -1  -1  -1  -1  -1  -1 ;; 63 Necromancer

;; Dragons
;; Base - 91
;; Eye Iris - 65
;; Dark Outline - 88
;; Horns - 21
;; Horns Shadow - 20
;; Throat - 90
;; Base Shadow - 36
;; Highlight - 92 ;Dark for Evil
;; Dark Underbelly - 26 ;Highlight for Evil
;; 91  -1  65  -1  88  -1  21  -1  90  -1  36  -1  92  -1  26  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;;  Dragon Template

;; Evil Dragons
   91  98  65  24  88  98  21  79  90  79  36  78  92  80  26  81  20  78  -1  -1  -1  -1  -1  -1 ;; 64 DragonA - Blue
   91  98  65  24  88  98  21  94  90  94  36  93  92  95  26  96  20  93  -1  -1  -1  -1  -1  -1 ;; 65 DragonB - Purple
   91  98  65  24  88  98  21  84  90  84  36  83  92  85  26  86  20  83  -1  -1  -1  -1  -1  -1 ;; 66 DragonC - Green
   91  98  65  24  88  98  21  59  90  59  36  58  92  60  26  61  20  58  -1  -1  -1  -1  -1  -1 ;; 67 DragonD - Red
   91  98  65  24  88  98  21  49  90  49  36  48  92  50  26  52  20  48  -1  -1  -1  -1  -1  -1 ;; 68 DragonE - Olive
   91  98  65  24  88  98  21  69  90  69  36  68  92  70  26  72  20  68  -1  -1  -1  -1  -1  -1 ;; 69 DragonF - Pink
   91  98  65  24  88  98  21  105  90  105  36  104  92  106  26  108  20  104  -1  -1  -1  -1  -1  -1 ;; 70 DragonG - Tan
   91  98  65  24  88  98  21  54  90  54  36  53  92  55  26  57  20  53  -1  -1  -1  -1  -1  -1 ;; 71 DragonH - Blue

;; Classic Dragons
   91  100  65  100  88  98  21  101  90  99  36  99  92  101  26  98  20  100  -1  -1  -1  -1  -1  -1 ;; 72 DragonI - Black
   91  85  65  52  88  83  21  51  90  83  36  84  92  50  20  50  -1  -1  -1  -1  -1  -1  -1  -1 ;; 73 DragonJ - Green
   91  75  36  74  88  73  90  74  92  73  21  74  26  76  20  73  -1  -1  -1  -1  -1  -1  -1  -1 ;; 74 DragonK - Brown
   91  76  36  75  88  74  90  105  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 75 DragonL - Tan
   91  54  36  53  26  98  88  98  92  100  65  56  21  100  90  53  20  99  -1  -1  -1  -1  -1  -1 ;; 76 DragonM - Blue


;; Slimes
;; Lightest - 57
;; Light - 56
;; Base - 55
;; Dark - 54
;; Darkest - 53
;; 53  -1  54  -1  55  -1  56  -1  57  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;;  Slime Template

   53  48  54  49  55  50  56  51  57  52  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 77 SlimeA - Olive
   53  58  54  59  55  60  56  61  57  62  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 78 SlimeB - Red
   53  63  54  64  55  65  56  66  57  67  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 79 SlimeC - Puke?
   53  68  54  69  55  70  56  71  57  72  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 80 SlimeD - Pink
   53  73  54  74  55  75  56  76  57  77  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 81 SlimeE - Brown
   53  78  54  79  55  80  56  82  57  82  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 82 SlimeF - Teal
   53  83  54  84  55  85  56  86  57  87  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 83 SlimeG - Green
   53  88  54  89  55  90  56  91  57  92  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 84 SlimeH - Orange
   53  93  54  94  55  95  56  96  57  97  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 85 SlimeI - Purple
   53  98  54  99  55  100  56  101  57  102  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 86 SlimeJ - Black
   53  104  54  105  55  106  56  107  57  108  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 87 SlimeK - Flesh
   53  98  54  98  55  58  56  60  57  62  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 88 SlimeL - Shiny Red
   53  98  54  98  55  78  56  70  57  72  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 89 SlimeM - Shiny Blue Pink
   53  98  54  98  55  73  56  75  57  77  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 90 SlimeN - Shiny Brown
   53  98  54  98  55  78  56  80  57  82  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 91 SlimeO - Shiny Teal
   53  98  54  98  55  83  56  85  57  87  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 92 SlimeP - Shiny Green
   53  98  54  98  55  88  56  90  57  92  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 93 SlimeQ - Shiny Orange
   53  98  54  98  55  93  56  95  57  97  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 94 SlimeR - Shiny Purple
   53  98  54  98  55  63  56  65  57  67  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 95 SlimeS - Shiny Yellow
   53  98  54  98  55  104  56  106  57  108  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 96 SlimeT - Shiny Flesh


;; Hell Souls
;; Light Flame (C. Eye) - 52
;; Base Flame - 65
;; Darker Flame - 91
;; Darkest Flame - 59
;; Eye Border - 45
;; Lightest Skull - 103
;; Lighter Skull - 102
;; Darker Skull - 101
;; Darkest Skull - 42
;; 52  -1  65  -1  91  -1  59  -1  45  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;;  Hell Soul Template

   52  57  65  55  91  54  59  53  45  98  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 97 HellSoulA - Blue 
   52  87  65  85  91  84  59  83  45  98  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 98 HellSoulB - Green
   52  67  65  65  91  64  59  63  45  98  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 99 HellSoulC - Yellow
   52  62  65  60  91  59  59  58  45  98  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 100 HellSoulD - Red
   52  82  65  80  91  79  59  78  45  98  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 101 HellSoulE - Teal
   52  97  65  95  91  94  59  93  45  98  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 102 HellSoulF - Purple
   52  73  65  71  91  70  59  69  45  98  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 103 HellSoulG - Pink
   52  16  65  43  91  57  59  31  45  43  103  98  102  99  101  100  42  54  -1  -1  -1  -1  -1  -1 ;; 104 HellSoulH - Dark Blue
   52  16  65  43  91  87  59  31  45  43  103  98  102  99  101  100  42  84  -1  -1  -1  -1  -1  -1 ;; 105 HellSoulH - Dark Green
   52  16  65  43  91  67  59  31  45  43  103  98  102  99  101  100  42  64  -1  -1  -1  -1  -1  -1 ;; 106 HellSoulH - Dark Yellow
   52  16  65  43  91  62  59  31  45  43  103  98  102  99  101  100  42  59  -1  -1  -1  -1  -1  -1 ;; 107 HellSoulH - Dark Red
   52  16  65  43  91  82  59  31  45  43  103  98  102  99  101  100  42  79  -1  -1  -1  -1  -1  -1 ;; 108 HellSoulH - Dark Teal
   52  16  65  43  91  97  59  31  45  43  103  98  102  99  101  100  42  94  -1  -1  -1  -1  -1  -1 ;; 109 HellSoulH - Dark Purple
   52  16  65  43  91  72  59  31  45  43  103  98  102  99  101  100  42  69  -1  -1  -1  -1  -1  -1 ;; 110 HellSoulH - Dark Pink

;; Flying Imps
;; Base - 58
;; Wing Top - 35 (Same as 58)
;; Base Highlight - 89
;; Base Shadow - 68
;; Wing Underneath - 69
;; Pitchfork Shaft - 129
;; Pitchfork - 41
;; Pitchfork Highlight - 115
;; Tongue - 71
;; Eyes - 86
;; 58  -1  89  -1  68  -1  69  -1  35  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;;  FlyingImp Template

   58  99  89  34  68  98  69  33  115  34  129  102  71  55  86  18  35  99  -1  -1  -1  -1  -1  -1 ;; 111 FlyingImpA - Black Green
   58  54  89  56  68  53  69  30  35  54  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 112 FlyingImpB - Blue
   58  84  89  86  68  83  69  34  35  84  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 113 FlyingImpC - Green
   58  94  89  96  68  93  69  97  35  94  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 114 FlyingImpD - Purple
   58  49  89  51  68  48  69  52  35  49  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 115 FlyingImpE - Olive
   58  59  89  61  68  58  69  62  35  59  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 116 FlyingImpF - Red
   58  64  89  66  68  63  69  67  35  64  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 117 FlyingImpG - Yellow
   58  69  89  71  68  68  69  72  35  69  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 118 FlyingImpH - Pink
   58  74  89  76  68  73  69  77  35  74  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 119 FlyingImpI - Brown
   58  79  89  81  68  78  69  82  35  79  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 120 FlyingImpJ - Teal
   58  89  89  91  68  88  69  92  35  89  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 121 FlyingImpK - Orange
   58  99  89  101  68  98  69  102  35  99  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 122 FlyingImpL - Black
   58  105  89  107  68  104  69  108  35  105  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 123 FlyingImpM - Flesh
   58  102  89  100  68  103  69  99  35  102  86  18  115  103  129  102  -1  -1  -1  -1  -1  -1  -1  -1 ;; 124 FlyingImpN - White

;; ----Baldrics
;; TSash
;; Sash Base - 85
;; Sash Highlight - 86
;; Sash Darker - 84
;; Sash Darkest - 83
;; Flower Base - 92
;; Flower Highlight - 22
;; Flower Darker - 91
;; Flower Line - 65
;; Flower Side - 66
;; Flower Side Highlight - 67
;; Flower Light Spots - 43
;; 
;; 86  -1  85  -1  84  -1  83  -1  92  -1  91  -1  22  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 

   86  99  85  98  84  80  83  82  92  80  91  79  22  82  65  79  66  79  67  80  -1  -1  -1  -1 ;; 125 ThistlebarkA
   86  57  85  56  84  55  83  98  92  70  91  69  22  72  65  69  66  69  67  70  -1  -1  -1  -1 ;; 126 ThistlebarkB
   ;;86  -1  85  -1  84  -1  83  -1  92  -1  91  -1  22  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 127 ThistlebarkC
   ;;86  -1  85  -1  84  -1  83  -1  92  -1  91  -1  22  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 128 ThistlebarkD
   ;;86  -1  85  -1  84  -1  83  -1  92  -1  91  -1  22  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 129 ThistlebarkE
   

;; ----------------------------------------------------------------------
;; index terminator DO NOT CHANGE!!!!
;; ALSO remember to update this index if more color entries are added.
   -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1 ;; 127

	]

;_______________________
;
;	scale table
;_______________________
;

	lReScaleTable = [
		100 100	; regular
		125  125	; orc
		 75  90	; elf
		150 150	; giant
		50  50	; pixie
		200 200	; huge demon
		125 125 ; slightly larger
		75 75 ; slightly smaller
		25 25 ; very tiny
		-1 -1
		-1 -1
		-1 -1
		-1 -1
		-1 -1
		-1 -1
		-1 -1
		-1 -1
		-1 -1
		-1 -1
		-1 -1
	]
)

(procedure (SetClut aClut aRange &tmp tIndex i)
	;; tIndex - which clut to use (clut index * 24)
	(= tIndex (* aRange 24))

	;mike-debug 
	;(chatDisplay add: (String format: "|c60|Doing clut %d , index starting: %d " aRange tIndex ) )

	(= i 0)

	;; test for -1 and 12 remappable colors (index * 2)
	(while
	(and
			(> tIndex -1)
			(> (+ tIndex 1) -1)
			(< tIndex (* NUM_COLOR_ENTRIES 24))
			(!= [lRemapTable tIndex] -1)
			(< i 12)
		)
			
		;mike-debug 
		;(chatDisplay add: (String format: "|c111|Remap %d to %d " [lRemapTable tIndex] [lRemapTable (+ tIndex 1)] ) )

		;insert remapped value into clut table
		(aClut at: [lRemapTable tIndex] [lRemapTable (++ tIndex)])
		(++ tIndex)
		(++ i)
	)
	;; send the clut back to the mutha

	(return aClut)
)


(class SetScale of Code
	(method (doit aWhatObj aRange &tmp tIndex i)
		;; tIndex - which scale to use (scale index * 2)

		(= tIndex (* aRange 2))

		(= i 0)

		;; test for -1 and the 2 rescaleable values (index * 2)

		(while
			(and
				(> tIndex -1)
				(> (+ tIndex 1) -1)
				(< tIndex (* NUM_SCALE_ENTRIES 2))
				(!= [lReScaleTable tIndex] -1)
				(< (++ i) 2)
			)
				;; rescale those bad boys!

			(aWhatObj xScaleAdj: [lReScaleTable tIndex]) 
			(aWhatObj yScaleAdj: [lReScaleTable (++ tIndex)]) 
		)

		(return)
	)
)


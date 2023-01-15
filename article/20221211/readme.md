# 核実験用バリアント馬鹿馬鹿蛮怒 --モンスター編1

本記事は[Roguelike Advent Calendar 2022](http://qiita.com "Roguelike Advent Calendar 2022")の11日目の記事です。
前回同様本来馬鹿馬鹿の公式Webにアップする予定でしたがとりあえずGitHubに記事フォルダを作り作成しました。そのうち移すかもしれません。

## なんか色々垂れ流すモンスター共

> F:SPAWN_*_X_IN_Y_KIND

というフラグ指定で、モンスターたちが一定確率ごとに色んなものを垂れ流す仕様を実装しました。これは色々おいしいと思うのでその内、変愚にもロンダリングして輸出してみたいと思います。

### モンスターを垂れ流すモンスター

シャークネードネタ。

```
N:1537:サメ台風
E:Sharknado
G:v:b
I:120:19d19:100:30:0
W:43:2:0:100:0:0
B:ENGULF:ACID:6d3
B:ENGULF:ACID:6d3
B:ENGULF:ACID:6d3
R:1472:3d5
F:PREVENT_SUDDEN_MAGIC | RAND_50 | WILD_SHORE | 
F:EMPTY_MIND | BASH_DOOR | POWERFUL | CAN_FLY | RES_WATE
F:IM_ACID | NO_FEAR | NO_CONF | NO_SLEEP | NO_STUN | NONLIVING
F:SPAWN_CREATURE_5_IN_16_1472
D:なんてこった！それは空飛ぶ鮫を運んでくる大型の台風だ！
```

```
N:1472:空飛ぶ鮫
E:Sky shark
G:l:b
I:112:20d10:20:35:30
W:21:3:0:300:0:0
B:BITE:HURT:3d15
B:BITE:HURT:3d15
F:PREVENT_SUDDEN_MAGIC | FORCE_MAXHP | CAN_FLY |
F:DROP_CORPSE
F:BASH_DOOR
F:RES_WATE | RES_TELE | RIDING
D:本来海の脅威であるはずの存在が、台風に乗って襲い来るぞ！チェンソーの用意はまだか！？
```

![サメ台風](./monster1.png)

### アイテムを垂れ流すモンスター

金の卵はランダムに能力値増強します。

```
N:1524:金のガチョウ
E:Gilded goose
G:B:y
I:110:3d5:40:12:0
W:2:100:0:10:10:68
B:BITE:HURT:1d6
B:BITE:HURT:1d6
F:ANIMAL | WILD_ONLY | CAN_FLY | DROP_CORPSE | FORCE_MAXHP | WILD_GRASS 
F:WILD_WOOD |
F:SPAWN_ITEM_7_IN_60000_743
D:くれぐれも苛立ちや欲に駆られて絞め殺したりしないことだ。
```

```
N:743:金の卵
E:& Golden Egg~
G:,:o
I:80:52:3000
A:40/5
W:40:5:2:30000
D:類稀なる金のガチョウが生み落とした卵だ。食えば精力が増す。
```

![金のガチョウ](./monster2.png)

### 地形を垂れ流すモンスター

（いつも通り）やったぜ。

```
N:1307:岡山の文豪『変態糞土方』
E:The Hentai Feces Labor, Literary Master of Okayama 
G:p:v
I:120:40d101:101:100:0
W:53:7:0:35000:0:0
B:TOUCH:DISEASE:8d8
B:TOUCH:DISEASE:8d8
B:ENEMA:DEFECATE:10d10
B:ENEMA:DEFECATE:10d10
R:1353:1d1
R:1354:1d1
F:UNIQUE | MALE | CAN_SPEAK | DROP_CORPSE | FRIENDLY | HAS_LITE_2 | HUMAN |
F:PREVENT_SUDDEN_MAGIC | FORCE_MAXHP | RES_TELE | ALLIANCE_DOKACHANS
F:ONLY_ITEM | DROP_2D2 | DROP_GREAT | CAN_SPEAK
F:SMART | OPEN_DOOR | BASH_DOOR | 
F:EVIL | NASTY | HOMO_SEXUAL | IM_POIS | NO_CONF | NO_SLEEP
F:SPAWN_FEATURE_1_IN_1_237
S:1_IN_2 | BR_NUKE
D:やったぜ。
```

```
N:237:DEEP_DUNG_POOL
J:深い糞溜め
E:deep dung pool
G:~:y
W:2
F:LOS | PROJECT | MOVE | PLACE | REMEMBER | DUNG_POOL | DEEP | CAN_FLY |
F:C-PRIORITY_50
F:TELEPORTABLE
F:HYGIENE_-80
```

![ああ＾～](./monster3.png)

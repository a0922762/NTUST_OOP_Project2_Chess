# Chess

這是一款西洋棋遊戲，雙擊`Chess.exe`即可開啟程式。

# 有實作的功能

- Move Piece  所有棋子能照預設規則移動 (5%)
- Special case 特殊規則 (15%)
    - Promoting (升階)	(5%)
    - Castle (入堡)		(5%)
    - En Passant(吃過路兵) 	(5%)
- Undo/Redo (5%)
- 顯示遊戲結果(20%)
    - Win/Lose   顯示誰勝利	(5%)
    - Draw	  遊戲和局	(5%)
    - Checkmate 將死		(10%)
- Menu	選單 (5%)
- Resign	投降 (5%)
- Timeout	時間到 (5%)
- Display Clock 顯示棋鐘 (2%)
- Who first	誰先手(3%)
- Load File	讀檔(5%) //For Fast demo FEN Standard

- 額外功能
    - 匯出為FEN
    - fifty move rule


# 設定

開啟程式後會跳出設定頁面，有兩種選項可以設定

1. 盤面
    - 標準盤面（預設） -- 經典的西洋棋盤面
    - 標準盤面（黑方先手） -- 同上，只不過黑方先走
    - FEN碼 -- 輸入FEN碼來載入盤面 （我們有加上Validator，防止玩家輸入錯太多的FEN碼，例如：出現奇奇怪怪的字元）

2. 時間
    - 沒有時間限制（預設） -- 棋鐘會紀錄你總共花多久時間思考
    - 有時間限制 -- 每人有固定的思考時間，用完即判輸 （若設成 0小時0分0秒 則為倒數24小時）

# 移動棋子

先點一顆棋，它會顯示走法提示（紅->可移；綠->可吃），點擊提示標記的其中一格即可移動

若點棋子後，隨便點一個它不能走的格子or其中一個移動方的棋子，即可改選擇移動其他顆

> 用左鍵長按並移動可拖動棋子。

# 勝負

若出現checkmate或無子可走，即判勝負

我們有實作fifty move rule，若halfmove達50（或以上）即判和局

你也可以點左上的`option -> surrender`來投降

# 遊戲中選單

你可在左上角看到遊戲中選單

1. option
    - New Game -- 跳回設定畫面，重新開始新遊戲
    - Pause -- 暫停
    - Surreder -- 投降
    - Exit -- 結束遊戲

2. Advance
    - To FEN -- 將目前盤面匯出為FEN
    - Undo -- 回到上一步
    - Redo -- 取消Undo（若Undo後有移動，則不能Redo）

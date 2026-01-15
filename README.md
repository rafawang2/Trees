# AVLtree
這是一個使用 C++ 實作的 AVL Tree，支援：

- 插入節點（Insert）
- 刪除節點（Delete）
- 自動平衡（LL、LR、RR、RL 旋轉）
- 計算 Height 與 Balance Factor
- 樹狀結構可視化列印

### 編譯方式
`cd ./AVLtree`

`g++ main.cpp -o main`

## 使用方式
程式分成兩個階段：

- 插入節點

- 刪除節點

每個階段皆輸入整數，直到輸入 -1 結束該階段。

範例操作
50 10 80 90 70 60 65 62 -1
70 10 -1


第一段：插入
第二段：刪除

每次操作後都會顯示目前 AVL Tree 結構。

輸出格式說明

每個節點顯示格式：

+-value(height, BF)

範例：

+-60(3, 0)

表示：

value = 60

height = 3

BF = 0（平衡）

樹狀結構以「旋轉 90 度」方式呈現，右子樹在上，左子樹在下。

## 功能說明
### 1. 節點結構
```
struct AVLnode {
    int value;
    int height;   // 從此節點到最深葉節點的距離
    int BF;       // Balance Factor = 左子樹高度 - 右子樹高度
    AVLnode* Lchild;
    AVLnode* Rchild;
};
```

### 2. 插入（Insert）
```
AVLnode* insert(AVLnode* cur, int val);
```

流程：

依 BST 規則插入

更新 Height 與 BF

判斷是否失衡

進行以下旋轉之一：

LL, LR, RR, RL

### 3. 刪除（Delete）
```
AVLnode* deleteNode(AVLnode* cur, int val);
```

支援三種情況：

Leaf（無子）

單子樹

雙子樹（使用右子樹最小值取代）

刪除後一樣會：

更新 Height / BF

進行必要旋轉以維持平衡

### 4. 旋轉操作

左旋（Left Rotate）
```
AVLnode* LeftRotate(AVLnode* node);
```
右旋（Right Rotate）
```
AVLnode* RightRotate(AVLnode* node);
```
### 5. 搜尋
```
AVLnode* search(int val);
```
使用 iterative 方式從 root 開始搜尋。
# Btree
本專案使用 C++ 實作 B-Tree，支援：
- 插入（Insert）

- 刪除（Delete）

- 搜尋（Search）

- 節點分裂（Split Child）

- 借鍵（Borrow / Fill）

- 合併（Merge）

- 樹結構列印（依層級輸出）

### 編譯方式
`cd ./Btree`

`g++ main.cpp -o main`

## 使用方式

程式分成兩個階段：

- 插入階段

- 刪除階段

每個階段輸入整數，直到輸入 -1 結束該階段。

範例輸入: 

`50 10 80 90 70 60 65 62 -1`

`50 10 80 90 70 60 65 62 -1`

第一行：插入

第二行：刪除

每次操作後都會印出整棵 B-Tree 的結構。

## 輸出格式說明

每個節點會顯示為：
```
level: 0 (50, 70)
level: 1 (10)
level: 1 (60, 65)
level: 1 (80, 90)
```
格式：

level: <層數> (key1, key2, key3, ...)

root 為 level 0

每往下一層 level +1

一個節點中可能有多個 key

## 核心設計
### 1. 節點結構
```
struct Bnode {
    vector<int> keys;         // 儲存 keys
    vector<Bnode*> children; // 子節點指標
};
```
每個節點最多：
2t-1 個 keys
，2t個children

### 2. B-Tree 參數
```
int t;        // 最小度數 (minimum degree)
int min_key;  // = t - 1
int max_key;  // = 2t - 1
```
在 main 中：

BTree BT = BTree(2);

代表：BT = Btree of order 2

最少 key = 1，最多 key = 3

## 功能說明
### Search
```
pair<Bnode*, int> Search(Bnode* cur, int val);
```

邏輯：

找第一個 >= val 的 key

若相等 → 回傳該節點與 index

若是 leaf → 找不到

否則遞迴往對應 child

### Insert
```
void insert(int val);
```

流程：

若 root 為空 → 建新 root

若 root 滿 → 先 split root

呼叫 `insertNonFull()`

### insertNonFull
```
void insertNonFull(Bnode* node, int val);
```

若是 leaf：直接插入正確位置

若是 internal node：

找要走的 child

若 child 滿 → 先 split

再往下遞迴

### Split Child
```
void splitchild(Bnode* parent, int child_idx);
```
步驟：
1. 中間 key 上升到 parent

2. 右半 keys 給 new_child

3. 左半 keys 留在原 child

4. children 也對半分

5. parent 插入 new_child 指標

### Delete
```
void remove(int val);
```

實際由：

`void deleteNode(Bnode* cur, int val);`
負責

### Delete 核心 Case
- Case 1：在 leaf 且 key > min_key

    → 直接刪

- Case 2：在 internal node

    - 2-a 左子有 ≥ t 個 key

        用 predecessor 取代

        遞迴刪 predecessor

    - 2-b 右子有 ≥ t 個 key

        用 successor 取代

        遞迴刪 successor

    - 2-c 左右都只有 min_key

        merge

        再往下刪

- Case 3：不在本層

    確保要走的 child 至少有 t 個 key

    不夠就 fill

    再往下遞迴

### Fill（補節點）
`void fill(Bnode* parent, int idx);`

三種情況：

1. 向左兄弟借

2. 向右兄弟借

3. 借不到就 merge

### Merge
`void merge(Bnode* parent, int idx);`

流程：

1. parent 的 key 下放

2. right child keys 接到 left

3. right child children 接到 left

4. parent 移除 key 與 child 指標

5. delete right child

### 主程式流程
```
while (cin >> n) { insert... }
while (cin >> n) { remove... }
```

你可以自由輸入測資，觀察：

split

borrow

merge

tree height 變化
# Huffman
# RedBlackTree

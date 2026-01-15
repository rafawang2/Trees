# AVLtree
這是一個使用 C++ 實作的 AVL Tree，支援：

- 插入節點（Insert）
- 刪除節點（Delete）
- 自動平衡（LL、LR、RR、RL 旋轉）
- 計算 Height 與 Balance Factor
- 樹狀結構圖形化列印

---

### 編譯方式
```cmd
cd ./AVLtree
g++ main.cpp -o main
```

---

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

---

## 功能說明
### 1. 節點結構
```cpp
struct AVLnode {
    int value;
    int height;   // 從此節點到最深葉節點的距離
    int BF;       // Balance Factor = 左子樹高度 - 右子樹高度
    AVLnode* Lchild;
    AVLnode* Rchild;
};
```

---

### 2. 插入（Insert）
```cpp
AVLnode* insert(AVLnode* cur, int val);
```

流程：

1. 依 BST 規則插入

2. 更新 Height 與 BF

3. 判斷是否失衡

4. 進行以下旋轉之一：

    LL, LR, RR, RL

---

### 3. 刪除（Delete）
```cpp
AVLnode* deleteNode(AVLnode* cur, int val);
```

支援三種情況：

1. Leaf（無子）

2. 單子樹

3. 雙子樹（使用右子樹最小值取代）

刪除後一樣會：

- 更新 Height / BF

- 進行必要旋轉以維持平衡

---

### 4. 旋轉操作

左旋（Left Rotate）
```cpp
AVLnode* LeftRotate(AVLnode* node);
```
右旋（Right Rotate）
```cpp
AVLnode* RightRotate(AVLnode* node);
```

---

### 5. 搜尋
```cpp
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

---

### 編譯方式
```cmd
cd ./Btree
g++ main.cpp -o main
```

---

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

---

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

---

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
# Huffman
Huffman Tree with Custom Min-Heap (C++)

本專案為一個 **Huffman Tree Encode System**，包含：
- 自製 **Min-Heap** (Min-Priority queue)
- Huffman Tree
- encode
- decode
- BFS Traversal
- 樹狀輸出
---
## 功能
- 依照 Huffman 演算法規則建樹
- 為每個字元產生對應 Huffman Code
- 支援 **一整串 bit string解碼**
- 提供 **樹狀列印**
- BFS 走訪節點

---
## 資料結構設計
### Huffnode
```cpp
struct Huffnode {
    Huffnode* Lchild;
    Huffnode* Rchild;
    char word;
    int freq;
    string code;
};
```
- `word`：字元

- `freq`：出現頻率

- `code`：Huffman 編碼

- `Lchild`, `Rchild`：左右子樹指標

### Min-Heap 實作

- `heapify`

- `heapify_up`

- `build_heap`

- `ExtractMinNode`

確保每次都能以 **O(log n)** 取出最小頻率節點，符合 Huffman 演算法需求。

## Huffman Tree 建構流程

將所有 (char, freq) 建立成 leaf nodes

建立 Min-Heap

重複：

1. 取出兩個最小 freq 節點

2. 合併成 parent node

3. push 回 heap 並做 heapify_up

4. 直到只剩一個節點作為 root

5. 產生編碼

## encode

左子樹：0

右子樹：1

只在葉節點儲存 code

```cpp
void encode(Huffnode* cur, string code) {
    if (cur == nullptr)
        return;
    if (cur->Lchild == nullptr && cur->Rchild == nullptr) {
        cur->code = code;
    }
    encode(cur->Lchild, code + "0");
    encode(cur->Rchild, code + "1");
}
```

## decode
支援解一整串 bit string，例如：
```cpp
HT.decode("111110101110101000100");
// output: edbacgf
```
---

### 編譯方式
```cmd
cd ./Huffman
g++ main.cpp -o main
```
---

### 範例輸入/輸出
```cpp
vector<pair<char,int>> datas = {
    {'a', 10},
    {'b', 50},
    {'c', 60},
    {'d', 62},
    {'e', 65},
    {'f', 70},
    {'g', 80},
};
```
```cpp
// output
====================
         +-(e, 65, 111)
      +-(127)
      |  +-(d, 62, 110)
   +-(247)
   |  |     +-(b, 50, 1011)
   |  |  +-(60)
   |  |  |  +-(a, 10, 1010)
   |  +-(120)
   |     +-(c, 60, 100)
+-(397)
   |  +-(g, 80, 01)
   +-(150)
      +-(f, 70, 00)
====================
```

---

# RedBlackTree
本專案使用 C++ 實作 Red-Black Tree（紅黑樹），支援：

- 插入（Insert）

- 刪除（Delete）

- 旋轉（Left / Right Rotate）

- 插入修正（Insert Fixup）

- 刪除修正（Delete Fixup）

- NIL 節點設計

- 樹狀結構圖像化（紅色節點顯示為紅色）

### 編譯方式
`cd ./ RedBlackTree`

`g++ main.cpp RedBlack_tree.cpp -o main`

## 使用方式

程式分成兩個階段：

- 插入節點

- 刪除節點

每個階段輸入整數，直到輸入 -1 結束該階段。

範例輸入：

`50 10 80 90 70 60 65 62 -1`

`50 10 80 90 70 60 65 62 -1`


第一行：插入，第二行：刪除

每次操作後都會顯示目前紅黑樹結構。

## 輸出格式說明

黑色節點：正常顯示

紅色節點：以 ANSI 紅色顯示

NIL 節點：顯示為 NIL

格式範例：
```
      +-NIL
   +-80
   |  +-NIL
+-10
```

紅色節點會顯示成紅色字體，方便除錯與理解結構。

## 核心設計
### 1. 節點結構
```
struct RBnode {
    int value;
    bool color;   // RED / BLACK
    RBnode* parent;
    RBnode* Lchild;
    RBnode* Rchild;
};
```
### 2. NIL 節點設計
```
RBnode* NIL;
```

所有 leaf 的 child 都指向 NIL

NIL 一律是 BLACK

## 功能說明
### Insert
```
void insert(RBnode* cur, int v);
```

流程：

1. 先用 BST 規則插入

2. 新節點顏色設為 RED

3. 呼叫 insert_fixup() 修正紅黑性質

### Insert Fixup
```
void insert_fixup(RBnode* cur);
```

處理四大情況：

**Case 說明**
- case 1 父與叔皆為 RED → 變色
- case 2 自己 & 父皆為 RED
    - LL	右旋
    - LR	左旋 + 右旋
    - RL	右旋 + 左旋
    - RR	左旋

### Rotate
```
void left_rotate(RBnode* x);
void right_rotate(RBnode* x);
```

同時處理：
```
parent 指標
root 更新
NIL 節點
```

### Search
```
RBnode* find(int val);
```
標準 BST 搜尋流程。

### Delete
```
void deleteNode(int val);
```

流程：

1. 找到節點 z

2. 分三種情況：

    - 左子為 NIL
    - 右子為 NIL
    - 兩邊都有 → 找 successor
3. 使用 transplant()

4. 若刪到 BLACK 節點 → 呼叫 delete_fixup()

### Delete Fixup
```
void delete_fixup(RBnode* x);
```
| Case | 說明 |
| :---: | :--- |
|Case 1 |兄弟為 RED|
|Case 2 |兄弟 BLACK + 兩子 BLACK|
|Case 3 |兄弟 BLACK + 近子 RED|
|Case 4 |兄弟 BLACK + 遠子 RED|

### Transplant
```
void transplant(RBnode* u, RBnode* v);
```

用途：刪除節點時，用**v接替u**的位置
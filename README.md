# AVLtree
這是一個使用 C++ 實作的 AVL Tree，支援：

- 插入節點（Insert）
- 刪除節點（Delete）
- 自動平衡（LL、LR、RR、RL 旋轉）
- 計算 Height 與 Balance Factor
- 樹狀結構可視化列印

### 編譯方式
g++ main.cpp -o main

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
1. 節點結構
    ```
    struct AVLnode {
        int value;
        int height;   // 從此節點到最深葉節點的距離
        int BF;       // Balance Factor = 左子樹高度 - 右子樹高度
        AVLnode* Lchild;
        AVLnode* Rchild;
    };
    ```

2. 插入（Insert）
    ```
    AVLnode* insert(AVLnode* cur, int val);
    ```

    流程：

    依 BST 規則插入

    更新 Height 與 BF

    判斷是否失衡

    進行以下旋轉之一：

    LL, LR, RR, RL

3. 刪除（Delete）
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

4. 旋轉操作

    左旋（Left Rotate）
    ```
    AVLnode* LeftRotate(AVLnode* node);
    ```
    右旋（Right Rotate）
    ```
    AVLnode* RightRotate(AVLnode* node);
    ```
5. 搜尋
    ```
    AVLnode* search(int val);
    ```
    使用 iterative 方式從 root 開始搜尋。
# Btree
# Huffman
# RedBlackTree

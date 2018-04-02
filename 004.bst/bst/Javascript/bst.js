const BST = function() {
    this._root = null;

    const Node = function(data) {
        if (!data) {
            return;
        }
        this.data = data;
        this.left = null;
        this.right = null;
        this.parent = null;
    };

    this.insert = function(data) {
        if (data === undefined) {
            return;
        }

        if (isNaN(data)) {
            return;
        }

        let node = new Node(data);

        let pNode = this._root;

        while (pNode !== null) {
            if (pNode.data >= data) {
                pNode = pNode.left;
            } else {
                pNode = pNode.right;
            }
        }

        pNode = node;

    };

    this.remove = function(data) {};

    this.find = function(data) {};

    this.max = function() {
        let pNode = this._root;
        while (pNode.right) {
            pNode = pNode.right;
        }
        return pNode.data;
    };

    this.min = function() {
        let pNode = this._root;
        while (pNode.left) {
            pNode = pNode.left;
        }
        return pNode.data;
    };

    this.pre = function(data) {};

    this.suc = function(data) {};

    this.preOrder = function() {};

    this.inOrder = function() {};

    this.posOrder = function() {};
};

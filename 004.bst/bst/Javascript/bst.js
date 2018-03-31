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

    this.insert = function(data) {};

    this.remove = function(data) {};

    this.find = function(data) {};

    this.max = function() {};

    this.min = function() {};

    this.pre = function(data) {};

    this.suc = function(data) {};

    this.preOrder = function() {};

    this.inOrder = function() {};

    this.posOrder = function() {};
};

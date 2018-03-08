const DoubleLinkedList = function() {
    this._root = null;
    this._tail = null;
    this._len = 0;

    const Node = function(data) {
        this.data = data;
        this.next = null;
        this.prev = null;
    };

    this.length = function() { return this._len; };

    this.head = function() { return this._root; };

    this.tail = function() { return this._tail; };

    this.addHead = function(data) {
        const node = new Node(data);
        if (this._root === null) {
            if (this._tail !== null) {
                throw new Error('Something went wrong /o\\');
            }
            this._root = node;
            this._tail = this._root;
        } else {
            node.next = this._root;
            this._root.prev = node;
            this._root = node;
        }
        ++this._len;
        return node;
    };

    this.addTail = function(data) {
        const node = new Node(data);
        if (this._tail === null) {
            if (this.root !== null) {
                throw new Error('Something went wrong /o\\');
            } else {
                this._tail = node;
                this._root = node;
            }
        } else {
            node.prev = this._tail;
            this._tail.next = node;
            this._tail = node;
        }
        ++this._len;
        return node;
    };

    this.popHead = function() {
        if (this._root === null) {
            return;
        }
        const node = this._root;

        this._root = node.next;

        if (this._root === null) {
            this._tail = this._root;
        } else {
            this._root.prev = null;
        }

        --this._len;
        node.prev = null;
        node.next = null;
        return node;
    };

    this.popTail = function() {
        if (this._tail === null) {
            return;
        }

        const node = this._tail;

        this._tail = node.prev;

        if (this._tail === null) {
            this._root = this._tail;
        } else {
            this._tail.next = null;
        }

        --this._len;
        node.next = null;
        node.prev = null;
        return node;
    };

    this.remove = function(data) {
        if (data === null || this._root === null) {
            return;
        }

        let pNode = this._root;
        let ppNode = this._root;

        while (pNode !== null) {
            if (pNode.data === data) {
                if (pNode === ppNode) {
                    this._root = pNode.next;
                    if (this._root === null) {
                        this._tail = null;
                    } else {
                        this._root.prev = null;
                    }
                } else {
                    if (pNode.next === null) {
                        this._tail = pNode.prev;
                    } else {
                        pNode.prev.next = pNode.next;
                        pNode.next.prev = pNode.prev;
                    }
                }
                --this._len;
                pNode.next = null;
                pNode.prev = null;
                return pNode;
            }
            ppNode = pNode;
            pNode = pNode.next;
        }
    };

    this.isEmpty = function() { return (this._len === 0); };

    this.print = function() {
        let pNode = this._root;
        console.log(`list len: ${this._len}`);
        while (pNode !== null) {
            console.log(pNode.data);
            pNode = pNode.next;
        }
    };

};

let list = new DoubleLinkedList();
list.addHead(1);
list.addHead(2);
list.addHead(3);
list.addHead(4);
list.print();
console.log(list.popHead());
console.log('----------------');
list.print();
console.log(list.remove(2));
console.log('----------------');
list.print();
console.log(list.popTail());
console.log('----------------');
list.print();
console.log(list.remove(3));
console.log('----------------');
list.print();

'use strict';

const LinkedList = function() {
    this._root = null;
    this._tail = null;
    this._len = 0;

    const Node = function(data) {
        this.data = data;
        this.next = null;
    }

    this.length = function() { return this._len; };

    this.head = function() { return this._root; };

    this.tail = function() { return this._tail; };

    this.add = function(data) {
        let node = new Node(data);
        if (this._root == null) {
            this._root = node;
        } else {
            let pNode = this._root;
            while (pNode.next !== null) {
                pNode = pNode.next;
            }
            pNode.next = node;
        }
        this._tail = node;
        ++this._len;
        return node;
    };

    this.remove = function(data) {
        let pNode = this._root;
        let ppNode = this._root;
        while (pNode !== null) {
            if (pNode.data === data) {
                if (pNode === ppNode) {
                    this._root = pNode.next;
                    if (this._root === null) {
                        this._tail = null;
                    }
                } else {
                    if (pNode.next === null) {
                        this._tail = ppNode;
                    }
                    ppNode.next = pNode.next;
                }
                --this._len;
                break;
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

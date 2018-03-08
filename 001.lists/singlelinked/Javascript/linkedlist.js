'use strict';

const LinkedList = function() {
    this._root = null;
    this._tail = null;
    this._len = 0;

    const Node = function(data) {
        if (!data) {
            return;
        }

        this.data = data;
        this.next = null;
    }

    this.length = function() { return this._len; };

    this.head = function() { return this._root; };

    this.tail = function() { return this._tail; };

    this.add = function(data) {
        if (data === null) {
            return;
        }

        let node = new Node(data);
        if (this._root == null) {
            this._root = node;
            this._tail = this._root;
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
                    }
                } else {
                    if (pNode.next === null) {
                        this._tail = ppNode;
                    }
                    ppNode.next = pNode.next;
                }
                --this._len;
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

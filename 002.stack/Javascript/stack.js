'use strict';

const Stack = function() {
    this._top = null;
    this._len = 0;

    const Node = function(data) {
        this.data = data;
        this.next = null;
    };

    this.pop = function() {
        if (this._top === null) {
            return;
        }

        const node = this._top;
        this._top = node.next;

        node.next = null;

        --this._len;

        return node;
    };

    this.push = function(data) {
        let node = new Node(data);

        if (this._top === null) {
            this._top = node;
        } else {
            node.next = this._top;
            this._top = node;
        }
        ++this._len;
    };

    this.size = function() {
        return this._len;
    };

    this.print = function() {
        let pNode = this._top;
        console.log(`stack len: ${this._len}`);
        while (pNode !== null) {
            console.log(pNode.data);
            pNode = pNode.next;
        }
    }
};

let stack = new Stack();
stack.push(1);
stack.push(2);
stack.push(3);
stack.push(4);
stack.print();
console.log(stack.pop());
stack.print();
console.log(stack.pop());
stack.print();
console.log(stack.pop());
stack.print();
console.log(stack.pop());
stack.print();
stack.push(4);
stack.print();
stack.push(3);
stack.print();
console.log(stack.pop());
stack.print();
stack.push(2);
stack.print();

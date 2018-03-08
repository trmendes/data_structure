const Queue = function() {
    this._root = null;
    this._tail = null;
    this._len = 0;

    const Node = function(data) {
        if (!data) {
            return;
        }

        this.data = data;
        this.next = null;
        this.prev = null;
    };

    this.queue = function(data) {
        let node = new Node(data);
        if (this._root === null) {
            this._root = node;
            this._tail = node;
        } else {
            this._root.prev = node;
            node.next = this._root;
            this._root = node;

        }
        ++this._len;
    };

    this.remove = function() {
        if (this._tail === null) {
            return;
        }

        let node = this._tail;

        this._tail = node.prev;

        if (this._tail === null) {
            this._root = null;
        } else {
            this._tail.next = null;
        }

        --this._len;

        node.next = null;
        node.prev = null;
        return node;
    };

    this.size = function() {
        return this._len;
    }

    this.print = function() {
        let pNode = this._root;
        while (pNode !== null) {
            console.log(pNode.data);
            pNode = pNode.next;
        }
    };
};

const queue = new Queue();
queue.queue(1);
queue.queue(2);
queue.queue(3);
queue.queue(4);
queue.queue(5);
queue.queue(6);
queue.print();

console.log(queue.remove());
queue.print();

console.log(queue.remove());
queue.print();

console.log(queue.remove());
queue.print();

console.log(queue.remove());
queue.print();

console.log(queue.remove());
queue.print();

console.log(queue.remove());
queue.print();

console.log(queue.remove());
queue.print();


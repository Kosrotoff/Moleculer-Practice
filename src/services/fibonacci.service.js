const {FibonacciSequence} = require('../../build/Release/FibonacciSequence');


module.exports = {
    name: 'fibonacci',
    actions: {
        getSequences(ctx) {
            return this.sequence.map(item => item.current());
        },
        add(ctx) {
            this.sequence.push(new FibonacciSequence());
        },
        next(ctx) {
            this.sequence[ctx.params.number].next();
        },
        reset(ctx) {
            if (!ctx.params.firstNumber) {
                ctx.params.firstNumber = 0;
            }
            if (!ctx.params.secondNumber) {
                ctx.params.secondNumber = 1;
            }

            this.sequence[ctx.params.number].reset(ctx.params.firstNumber, ctx.params.secondNumber);
        },
        delete(ctx) {
            this.sequence.splice(ctx.params.number, 1);
        },
        deleteAll(ctx) {
            this.sequence = [];
        }
    },
    created() {
        this.sequence = [];
    },
};

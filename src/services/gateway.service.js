const express = require("express");
const handlebars = require('express-handlebars');


module.exports = {
    name: 'gateway',
    settings: {
        server: false,
        port: process.env.port || 3000,
    },
    methods: {
        getRouter() {
            const router = express.Router();

            router.get('/', async (request, response) => {
                response.render('content/fibonacci-sequences', {
                    data: await this.broker.call('fibonacci.getSequences'),
                });
            });
            router.get('/fibonacci-add', async (request, response) => {
                await this.broker.call('fibonacci.add', { number: request.body.id });
                response.redirect('/');
            });
            router.get('/fibonacci-delete-all', async (request, response) => {
                await this.broker.call('fibonacci.deleteAll');
                response.redirect('/');
            });

            router.post('/fibonacci-next', async (request, response) => {
                await this.broker.call('fibonacci.next', { number: request.body.id });
                response.redirect('/');
            });
            router.post('/fibonacci-reset', async (request, response) => {
                await this.broker.call('fibonacci.reset', { number: request.body.id });
                response.redirect('/');
            });
            router.post('/fibonacci-delete', async (request, response) => {
                await this.broker.call('fibonacci.delete', {number: request.body.id});
                response.redirect('/');
            });

            return router;
        }
    },
    created() {
        const app = express();

        app.engine('hbs', handlebars({
            defaultLayout: 'main',
            extname: 'hbs',
        }));

        app.set('view engine', 'hbs');
        app.set('views', 'src/public/views');

        app.use(express.urlencoded({ extended: true, }));
        app.use(express.static('src/public/styles'));
        app.use(this.getRouter());

        this.app = app;
    },
    started() {
        this.app.listen(this.settings.port, error => {
            if (error) {
                return this.broker.fatal(error);
            }

            this.logger.info(`Server started on http://localhost:${this.settings.port}`);
        });
    },
    stopped() {
        if (this.app.listening) {
            this.app.close(error => {
                if (error) {
                    return this.logger.error("Server stop error", error);
                }

                this.logger.info("Server stopped");
            });
        }
    },
};

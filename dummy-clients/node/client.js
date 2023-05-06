// Require the framework and instantiate it
const fastify = require('fastify')({ logger: { enabled: true, level: 'debug' } })

// Declare a route
fastify.get('/', async (request, reply) => {
    return { hello: 'world' }
})

// Run the server!
const start = async () => {
    try {
        await fastify.listen({ port: 3000, host: '127.0.0.1' })
    } catch (err) {
        fastify.log.error(err)
        process.exit(1)
    }
}
start()
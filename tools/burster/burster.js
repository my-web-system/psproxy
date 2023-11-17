const axios = require('axios')

const bpcRoutes = [
    "/bpc",
    "/bpc/greet",
    "/bpc/greet/jordan",
    "/bpc/" // invalid
]
const delay = 10
const host = "http://localhost:8080"
idx = 0

function sleep(ms) {
    return new Promise((resolve) => {
        setTimeout(resolve, ms);
    });
}

async function call(route) {
    console.log(host + route)
    return new Promise((resolve, reject) => {
        axios.get(host + route)
            .then((res) => {
                console.log(res.data)
                resolve()
            })
            .catch((err) => console.log(err))
    })
}
(async function burster() {
    while (true) {
        await call(bpcRoutes[idx])
        idx = (idx === bpcRoutes.length - 1) ? 0 : idx + 1
        await sleep(delay)
    }
})()

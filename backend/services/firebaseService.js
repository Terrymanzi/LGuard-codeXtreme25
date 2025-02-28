const admin = require("firebase-admin");
const fs = require("fs");

const serviceAccount = JSON.parse(fs.readFileSync(process.env.FIREBASE_CREDENTIALS));

admin.initializeApp({
  credential: admin.credential.cert(serviceAccount),
  databaseURL: "https://your-firebase-project.firebaseio.com",
});

const db = admin.database();

const getAccidentData = async () => {
  const ref = db.ref("accidents");
  return new Promise((resolve, reject) => {
    ref.once("value", (snapshot) => {
      resolve(snapshot.val());
    }, reject);
  });
};

module.exports = { getAccidentData };

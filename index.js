//I'm not great at JS, so this might be spaghetti code lol...
const discord = require('discord.js');
//Appearantly this is a popular module to not leak your bot token
const dotenv = require('dotenv');


//Events seems to be an enum with a list of possible events to select, the second argument seems to just be a basic lambda
discord.once(Events.ClientReady, (event_data) =>{

     

});




discord.login(/*TOKEN GOES HERE*/);
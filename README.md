# **ft_IRC**

## Description
<table>
  <tr>
    <td>
      <img src="https://github.com/ayogun/42-project-badges/blob/main/badges/ft_ircm.png" alt="ft_IRC 42 project badge" width="400"/>
    </td>
    <td>
      ft_IRC is a 42 school team project where you need to recreate from scratch an <a href="http://users.atw.hu/wolf3d/">IRC</a> server, according to the .
      Coding in C++98, this server should follow the standard protocols such as <a href="https://www.rfc-editor.org/rfc/rfc2812">RFC 2812</a> and <a href="https://www.rfc-editor.org/rfc/rfc1459.html">RFC 1459</a>. 
    </td>
  </tr>
</table>

## :memo: Status
<p align="center">
  <br><strong>Status = 95/100% finished</strong>
  <br>All bonuses
</p>

## Features
There are two types of requirements: mandatory and bonus.
### :orange_book: Mandatory
+ Handle several client simultaneously without blocking :arrow_forward: Tested with 100+ connexions
+ Using a reference client with no errors :arrow_forward: We chose <a href="https://hexchat.readthedocs.io/fr/latest/index.html">Hexchat</a>
+ Provide user **authentification**, **channel management** and **operators**
+ Handle packet concatenation
+ Reproduce the comportment of an official IRC server, following the standard protocols, on the commands available

### :fire:  Bonus
+ File sending works
+ Creating a bot

## :cyclone: Clone and compile
Clone the repository and enter it :
```shell
git clone https://github.com/sponthus/IRC
cd IRC
make
```

## 	:runner: Run
From the project directory, enter :
```shell
./ircserv [port] [password]
```

During execution, you will have access to the server logs.  
By default, the server will be hosted at `localhost:[port]`.

## :computer: Server commands handled
* `PASS` allows you to enter server password
* `NICK` allows you to give your nickname or change it later
* `USER` allows you to give your username
* `JOIN` allows you to join 1 or more channel(s), or create them if they don't exist
* `PART` allows you to leave 1 or more channel(s)
* `PRIVMSG` allows you to send a message, to a channel or a specific nickname
* `KICK` allows you to kick someone from a channel
* `INVITE` allows you to invite someone to a channel you're in
* `TOPIC` allows you to either see the topic of a channel, or change it
* `WHO` with a channel, allows you to see who is connected on a channel
* `MODE` with options to add/remove:
    `i`: invitation only channel
    `k`: key-protected channel
    `o`: add/remove operator privilege on channel
    `l`: user limit on channel
* `QUIT` leaves the server, you can add a message
  

## :question: Bot
The bot can be launched with :
```shell
./QuestBot [port] [password]
```
Where port is the port of the server at `localhost`, and the password from the IRC server.

This bot creats its own channel, or joins it if already existing, and starts asking random questions if players are connected to the channel. He recognizes the answers given. You will have access to the bot logs during execution.

You will find an example of questions the bot can learn at [/capitals.txt](/). 
Questions can be formatted by following:
* Begins with `theme=[theme]`
* Followed by questions formatted as : `[question ?]|[answer(s)]`
* You can add 1 or more answers, using : `[answer1]/[answer2]/...`
* Answers accepted will include capitalized or non-capitalized answers
You can teach questions to the bot by using :
```shell
LEARN [question_file]
```
In the bot program.

:hugs: Enjoy !
---
Made by endoliam and sponthus

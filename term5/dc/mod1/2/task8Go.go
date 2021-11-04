// Variant 8

package main

import (
	"fmt"
	"math/rand"
	"sync"
	"time"
)

var (
	random			 	= rand.New(rand.NewSource(time.Now().UnixNano()))
	maxCashboxMoney 	= 1000
	partCashboxMoney 	= 500
	maxStorageMoney     = 10000
	mutex 				= &sync.Mutex{}
	clientAccountId 	= -1
)

type Storage struct {
	money int
}

func newStorage() *Storage {
	return &Storage{
		money: random.Intn(maxStorageMoney) + 1000,
	}
}

type Cashbox struct {
	money int
}

func newCashbox() Cashbox {
	return Cashbox{
		money: random.Intn(maxCashboxMoney) + 100,
	}
}

type ClientAccount struct {
	id 		int
	money 	int
}

func newClientAccount() ClientAccount {
	clientAccountId++

	return ClientAccount{
		id: clientAccountId,
		money: random.Intn(100),
	}
}

type Bank struct {
	storage 		*Storage
	cashboxes 		[]Cashbox
	clientAccounts 	[]ClientAccount
}

func (b *Bank) observe() {
	for i := range b.cashboxes {
		mutex.Lock()

		if b.storage.money < 0 {
			panic("Not enough money in the bank!")
		}

		if b.cashboxes[i].money <= 0 {
			if b.storage.money >= maxStorageMoney {
				b.cashboxes[i].money += maxCashboxMoney
			}
		}

		if b.cashboxes[i].money >= maxCashboxMoney {
			b.storage.money += partCashboxMoney
			b.cashboxes[i].money -= partCashboxMoney
		}

		mutex.Unlock()
	}
}

func newBank() *Bank {
	cashboxes := make([]Cashbox, 10)
	clientAccounts := make([]ClientAccount, 20)

	for i := range cashboxes {
		cashboxes[i] = newCashbox()
	}

	for i := range clientAccounts {
		clientAccounts[i] = newClientAccount()
	}

	return &Bank{
		storage: newStorage(),
		cashboxes: cashboxes,
		clientAccounts: clientAccounts,
	}
}

func (b *Bank) getCashbox() *Cashbox {
	return &b.cashboxes[random.Intn(len(b.cashboxes))]
}

func (b *Bank) withdrawMoney(id, amount int) {
	mutex.Lock()

	if b.clientAccounts[id].money - amount >= 0 {
		b.clientAccounts[id].money -= amount
		b.getCashbox().money += amount
	}

	mutex.Unlock()
}

func (b *Bank) replenishMoney(id, amount int) {
	mutex.Lock()

	b.getCashbox().money -= amount
	b.clientAccounts[id].money += amount

	mutex.Unlock()
}

func (b *Bank) transferMoney(id, otherClientId, amount int) {
	mutex.Lock()

	if b.clientAccounts[id].money >= amount {
		b.clientAccounts[id].money -= amount
		b.clientAccounts[otherClientId].money += amount
	}

	mutex.Unlock()
}

func (b *Bank) pay(id, amount int) {
	mutex.Lock()

	if b.clientAccounts[id].money - amount >= 0 {
		b.clientAccounts[id].money -= amount
		b.getCashbox().money += amount
	}

	mutex.Unlock()
}

func (b *Bank) serveClients(clients []Client) {
	for i := range clients {
		clients[i].work(b)
	}

	rand.Shuffle(len(clients), func(i, j int) {
		clients[i], clients[j] = clients[j], clients[i]
	})
}

type Client struct {
	id int
}

func newClient(id int) Client {
	return Client{
		id: id,
	}
}

func (c *Client) withdrawMoney(b *Bank) {
	amount := random.Intn(10)
	fmt.Printf("Client %v withdraw %v\n", c.id, amount)
	b.withdrawMoney(c.id, amount)
}

func (c *Client) replenishMoney(b *Bank) {
	amount := random.Intn(10)
	fmt.Printf("Client %v replenish %v\n", c.id, amount)
	b.replenishMoney(c.id, amount)
}

func (c *Client) transferMoney(b *Bank, ) {
	amount := random.Intn(10)
	otherClientId := random.Intn(clientAccountId)
	fmt.Printf("Client %v trasfer to %v amount %v\n", c.id, otherClientId, amount)
	b.transferMoney(c.id, otherClientId, amount)
}

func (c *Client) pay(b *Bank) {
	amount := random.Intn(10)
	fmt.Printf("Client %v pay %v\n", c.id, amount)
	b.pay(c.id, amount)
}

func (c *Client) work(b *Bank) {
	decision := rand.Intn(4)

	switch decision {
	case 0:
		c.withdrawMoney(b)
		break

	case 1:
		c.replenishMoney(b)
		break

	case 2:
		c.transferMoney(b)
		break

	case 3:
		c.pay(b)
		break
	}

	time.Sleep(time.Millisecond * time.Duration(500 + rand.Intn(700)))
}

func main() {
	bank := newBank()
	clients := make([]Client, clientAccountId + 1)

	for i := range clients {
		clients[i] = newClient(i)
	}

	go bank.observe()
	go bank.serveClients(clients)

	for {}
}

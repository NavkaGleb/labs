package main

type ClientId uint32

type Client struct {
	Id ClientId
}

func (client *Client) WaitForBarber(barber *Barber) {
	barber.IsFree.L.Lock()
	barber.IsFree.Wait()
	barber.IsFree.L.Unlock()
}

func (client *Client) Run(room *WaitingRoom, barber *Barber) {
	room.AddClient(client.Id)

	client.WaitForBarber(barber)
}

func NewClient(id ClientId) *Client {
	return &Client{
		Id: id,
	}
}

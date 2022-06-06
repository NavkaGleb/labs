import { Component, Input, OnInit } from '@angular/core';
import { FormBuilder, FormGroup, Validators } from '@angular/forms';
import { forkJoin, of } from 'rxjs';
import { User } from '../../../shared/dto/user';
import { OrderReadDto } from '../../../shared/dto/order';
import { OrderService, UserService } from '../../../service';

@Component({
  selector: 'app-order',
  templateUrl: './order.component.html',
  styleUrls: ['./order.component.scss']
})
export class OrderComponent implements OnInit {
  @Input() order: OrderReadDto;
  @Input() user: User & { isAdmin: boolean };

  orderForm: FormGroup;
  submitted = false;
  errorMessage: string = null;
  successfulMessage: string = null;

  private readonly prices: Record<string, number> = {
    'Naggets': 8,
    'Diablo': 30,
    'Chicken': 10,
    'Margarita': 15,
    'Stripces':6,
    'Neapolitana':25,
    'Big Tasty':20,
    'Four cheeses':30,
    'Milkshake':10,
    'Chicken roll':20,
    'Coca-cola':5,
    'BigMac': 10
  };

  constructor(private readonly formBuilder: FormBuilder, private readonly userService: UserService,
    private readonly orderService: OrderService) { }

  get f() { return this.orderForm.controls; }

  ngOnInit(): void {
    this.orderForm = this.formBuilder.group({
      status: ['', [Validators.required]],
    });

    this.orderForm.get('status').setValue(this.order.status);
  }

  private findInvalidControls(): any[] {
    const invalid = [];
    const controls = this.orderForm.controls;
    for (const name in controls) {
      if (controls[name].invalid) {
        invalid.push(name);
      }
    }
    return invalid;
  }

  onSubmit() {
    this.successfulMessage = null;
    this.errorMessage = null;
    this.submitted = true;
    if (this.orderForm.invalid) {
      console.log(this.findInvalidControls());
      return;
    }
    const newStatus = this.f.status.value;
    this.orderService.changeStatus(newStatus, this.order.id).subscribe(
      (data) => {
        this.successfulMessage = 'Order status was changed successfully';
        this.errorMessage = null;
      },
      (err) => {
        this.errorMessage = err;
        this.successfulMessage = null;
      });
  }
}

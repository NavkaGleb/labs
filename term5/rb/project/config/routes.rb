Rails.application.routes.draw do
  root "schoolboys#index"

  resources :schoolboys

  get "/task1" => "schoolboys#task1"
  get "/task2" => "schoolboys#task2"
  get "/task3" => "schoolboys#task3"
  get "/task4" => "schoolboys#task4"
  get "/random_schoolboys" => "schoolboys#create_random"
end

Rails.application.routes.draw do
  # root to: "users#index"

  root to: 'home#index'
  get 'home/index'

  devise_scope :user do
    get "/sign_in" => "devise/sessions#new" # custom path to login/sign_in
    get "/sign_up" => "devise/registrations#new", as: "new_user_registration" # custom path to sign_up/registration
  end

  # devise_for :users, controllers: { sessions: 'users/sessions' }
  # devise_for :users,
  #            controllers: { omniauth_callbacks: 'users/omniauth_callbacks',
  #                           registrations: 'users/registrations',
  #                           sessions: 'users/sessions',
  #                           passwords: 'users/passwords',
  #                           confirmations: 'users/confirmations'}

  devise_for :users,
             controllers: { registrations: 'users/registrations',
                            sessions: 'users/sessions',
                            passwords: 'users/passwords',
                            confirmations: 'users/confirmations'}

  get 'email-verification' => "home#email_verification"

  # as :user do
    get 'what-ever-you-want', :to => 'users#index', :as => :user_root # Rails 3
  # end

  # http://www.rubydoc.info/github/plataformatec/devise/ActionDispatch%2FRouting%2FMapper%3Adevise_scope
  # Also be aware of that 'devise_scope' and 'as' use the singular form of the noun where other devise route commands expect the plural form

  # devise_scope :users do # should use users
  #   get 'users/success' => 'users/registrations#success'
  #   get 'users/sign_in' => redirect('/')
  #   post 'users/check_email' => 'users#check_email'
  # end
  #
  # devise_scope :user do
  #   post 'users/sign_up' => 'users/registrations#create'
  # end

  resources :products do
    resources :product_comments
  end

  get  '/signup',  to: 'users#new'
  resources :users
  # For details on the DSL available within this file, see http://guides.rubyonrails.org/routing.html
end

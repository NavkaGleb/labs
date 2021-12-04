class SchoolboysController < ApplicationController
  def index
    @schoolboys = Schoolboy.all.order(created_at: :desc)
  end

  def show
    @schoolboy = Schoolboy.find(params[:id])
  end

  def new
    @schoolboy = Schoolboy.new
  end

  def create
    @schoolboy = Schoolboy.new(schoolboy_params)

    if @schoolboy.save
      redirect_to root_path
    else
      render :new
    end
  end

  def edit
    @schoolboy = Schoolboy.find(params[:id])
  end

  def update
    @schoolboy = Schoolboy.find(params[:id])

    if @schoolboy.update(schoolboy_params)
      redirect_to root_path
    else
      render :edit
    end
  end

  def destroy
    @schoolboy = Schoolboy.find(params[:id])

    @schoolboy.destroy

    redirect_to root_path
  end

  def task1
    @schoolboys = Schoolboy.where("where_want_to_enter LIKE 10").order(created_at: :desc)
  end

  def task2
    @schoolboys = Schoolboy.where("where_want_to_enter NOT LIKE 10").order(created_at: :desc)
  end

  def task3
    @schoolboys = Schoolboy.where("where_entered LIKE 10").order(created_at: :desc)
  end

  def task4
    schoolboy_count = Schoolboy.where("where_entered LIKE 10").order(created_at: :desc).count

    @schoolboys = []

    while schoolboy_count >= 25
      @schoolboys.append(25)
      schoolboy_count -= 25
    end

    if schoolboy_count != 0
      @schoolboys.append(schoolboy_count)
    end

    @schoolboys
  end

  def create_random
    @schoolboys = []

    schoolboy_count = rand(20) + 20

    (0..schoolboy_count).each do
      surname = (0...8).map { ('a'..'z').to_a[rand(26)] }.join
      where_want_to_enter = Schoolboy.get_valid_enter_establishments[rand(3)]
      where_entered = Schoolboy.get_valid_enter_establishments[rand(3)]

      schoolboy = Schoolboy.new(
        surname: surname,
        where_want_to_enter: where_want_to_enter,
        where_entered: where_entered
      )

      schoolboy.save

      @schoolboys.append(schoolboy)
    end

    redirect_to root_path
  end

  private
  def schoolboy_params
    params.require(:schoolboy).permit(:surname, :where_want_to_enter, :where_entered)
  end
end

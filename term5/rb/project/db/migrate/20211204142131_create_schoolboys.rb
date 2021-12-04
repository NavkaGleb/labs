class CreateSchoolboys < ActiveRecord::Migration[6.1]
  def change
    create_table :schoolboys do |t|
      t.string :surname
      t.string :where_what_to_enter
      t.string :where_entered

      t.timestamps
    end
  end
end

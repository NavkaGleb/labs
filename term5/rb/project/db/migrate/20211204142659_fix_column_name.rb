class FixColumnName < ActiveRecord::Migration[6.1]
  def change
    rename_column :schoolboys, :where_what_to_enter, :where_want_to_enter
  end
end

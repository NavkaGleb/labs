class Schoolboy < ApplicationRecord
  validates :surname, presence: true, length: { minimum: 2 }

  VALID_ENTER_ESTABLISHMENT = %w[10 knu kpi nau]

  validates :where_want_to_enter, presence: true, inclusion: { in: VALID_ENTER_ESTABLISHMENT }
  validates :where_entered, presence: true, inclusion: { in: VALID_ENTER_ESTABLISHMENT }

  def self.get_valid_enter_establishments
    %w[10 knu kpi nau]
  end
end

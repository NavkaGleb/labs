# Variant 8

class Phone
  attr_accessor :last_name,
                :first_name,
                :patronymic,
                :address,
                :card_number,
                :debit,
                :credit,
                :city_time,
                :international_time

  def initialize(last_name, first_name, patronymic, address, card_number, debit, credit, city_time, international_time)
    @last_name = last_name
    @first_name = first_name
    @patronymic = patronymic
    @address = address
    @card_number = card_number
    @debit = debit
    @credit = credit
    @city_time = city_time
    @international_time = international_time
  end

  def to_s
    result = ""

    result << "id:                 #{@id}\n"
    result << "last_name:          #{@last_name}\n"
    result << "first_name:         #{@first_name}\n"
    result << "patronymic:         #{@patronymic}\n"
    result << "address:            #{@address}\n"
    result << "card_number:        #{@card_number}\n"
    result << "debit:              #{@debit}\n"
    result << "credit:             #{@credit}\n"
    result << "city_time:          #{@city_time}\n"
    result << "international_time: #{@international_time}"

    result
  end
end

class PhoneController
  def self.filter_city_time(phones, city_time)
    phones.each do |phone|
      if phone.city_time > city_time
        print "#{phone}\n\n"
      end
    end
  end

  def self.filter_international_time(phones)
    phones.each do |phone|
      if phone.international_time != 0
        print "#{phone}\n\n"
      end
    end
  end

  def self.filter_alphabet_order(phones)
    phones_copy = phones.map(&:clone)

    phones_copy.sort! { |a, b| a.last_name <=> b.last_name }

    phones_copy.each { |phone| print "#{phone}\n\n" }
  end
end

def main
  phone1 = Phone.new("person1_last_name", "person1_first_name", "person1_patronymic", "123", "1111 1111 1111 1111", 1, 10000, 1001, 1001)
  phone2 = Phone.new("person2_last_name", "person2_first_name", "person2_patronymic", "123", "1111 1111 1111 1122", 2, 20000, 2002,    0)
  phone3 = Phone.new("person3_last_name", "person3_first_name", "person3_patronymic", "123", "1111 1111 1111 1333", 3, 30000, 3003, 3003)
  phone4 = Phone.new("person4_last_name", "person4_first_name", "person4_patronymic", "123", "1111 1111 1111 4444", 4, 40000, 4004,    0)
  phone5 = Phone.new("person5_last_name", "person5_first_name", "person5_patronymic", "123", "1111 1111 1115 5555", 5, 50000, 5005, 5005)

  phones = [
    phone3,
    phone2,
    phone5,
    phone1,
    phone4,
  ]

  puts "Filter by city time"
  PhoneController.filter_city_time(phones, 3000)
  puts "----------------------------"

  puts "Filter by international time"
  PhoneController.filter_international_time(phones)
  puts "----------------------------"

  puts "Filter by alphabet order"
  PhoneController.filter_alphabet_order(phones)
end

main

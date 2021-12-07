class Contact
  attr_reader :firstname
  attr_reader :surname
  attr_reader :phone_number

  def initialize(firstname, surname, phone_number)
    @firstname = firstname
    @surname = surname
    @phone_number = phone_number
  end
end

class Email < Contact
  attr_reader :email

  def initialize(firstname, surname, phone_number, email)
    super(firstname, surname, phone_number)

    @email = email
  end

  def to_s
    @firstname + " " + @surname + " { phone_number: " + @phone_number + ", email: " + @email + " }"
  end
end

class Skype < Email
  attr_reader :skype_id

  def initialize(firstname, surname, phone_number, email, skype_id)
    super(firstname, surname, phone_number, email)

    @skype_id = skype_id
  end

  def to_s
    @firstname + " " + @surname + " { phone_number: " + @phone_number + ", email: " + @email + ", skype_id: " + @skype_id + " }"
  end
end

class ContactManager
  def self.filter_by_firstname(contacts, string)
    result = []

    contacts.each do |contact|
      if contact.firstname.include? string
        result.append(contact)
      end
    end

    result
  end

  def self.filter_by_surname(contacts, string)
    result = []

    contacts.each do |contact|
      if contact.surname.include? string
        result.append(contact)
      end
    end

    result
  end

  def self.sort_by_surname(contacts)
    contacts.sort { |lhs, rhs| lhs.surname <=> rhs.surname }
  end
end

def log_contacts(contacts)
  contacts.each do |contact|
    puts contact
  end
end

def main
  contacts = [
    Email.new("sasha", "sashov", "000", "firstname1@knu.ua"),
    Skype.new("petya", "petrov", "111", "skype@gmail.com", "skype_id_1"),
  ]

  firstname_filter = ContactManager.filter_by_firstname(contacts, "s")
  surname_filter = ContactManager.filter_by_surname(contacts, "ov")
  surname_sorted_filter = ContactManager.sort_by_surname(contacts)

  puts "firstname_filter"
  log_contacts(firstname_filter)
  puts

  puts "surname_filter"
  log_contacts(surname_filter)
  puts

  puts "surname_sorted_filter"
  log_contacts(surname_sorted_filter)
  puts
end

main

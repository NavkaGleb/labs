package util;

public class Plane {

    public enum Type {
        ESCORT,
        SUPPORT,
        FIGHTER,
        INTERCEPTOR,
        SCOUT
    }

    public static class Specifications {

        private Type    type;
        private int     seatCount;
        private int     missileCount;

        public Specifications() {}

        public Specifications(Type type, int seatCount, int missileCount) {
            this.type = type;
            this.seatCount = seatCount;
            this.missileCount = missileCount;
        }

        public Type getType() {
            return type;
        }

        public void setType(Type type) {
            this.type = type;
        }

        public int getSeatCount() {
            return seatCount;
        }

        public void setSeatCount(int seatCount) {
            this.seatCount = seatCount;
        }

        public int getMissileCount() {
            return missileCount;
        }

        public void setMissileCount(int missileCount) {
            this.missileCount = missileCount;
        }
    }

    private String          model;
    private String          origin;
    private Specifications  specifications;
    private boolean         hasRadar;

    public Plane() {
        model = "";
        origin = "";
        specifications = new Specifications();
    }

    public Plane(String model, String origin, Specifications specifications, boolean hasRadar) {
        this.model = model;
        this.origin = origin;
        this.specifications = specifications;
        this.hasRadar = hasRadar;
    }

    public String getModel() {
        return model;
    }

    public void setModel(String model) {
        this.model = model;
    }

    public String getOrigin() {
        return origin;
    }

    public void setOrigin(String origin) {
        this.origin = origin;
    }

    public Specifications getSpecifications() {
        return specifications;
    }

    public void setSpecifications(Specifications specifications) {
        this.specifications = specifications;
    }

    public boolean hasRadar() {
        return hasRadar;
    }

    public void setRadar(boolean hasRadar) {
        this.hasRadar = hasRadar;
    }

    @Override
    public String toString() {

        return "Model: " + model + '\n' +
                "Origin: " + origin + '\n' +
                "Specifications:\n" +
                "\tType: " + specifications.type + '\n' +
                "\tSeatCount: " + specifications.seatCount + '\n' +
                "\tMissileCount: " + specifications.missileCount + '\n' +
                "HasRadar: " + hasRadar;
    }
}
CREATE TABLE "customers"
(
  "customer-id" INTEGER PRIMARY KEY AUTOINCREMENT,
  "first-name" TEXT,
  "last-name" TEXT,
  "address" TEXT,
  "city" TEXT,
  "state" TEXT,
  "zip" TEXT
);

CREATE TABLE "orders"
(
  "order-id" INTEGER PRIMARY KEY AUTOINCREMENT,
  "customer-id" INTEGER INTEGER REFERENCES "customers" ("customer-id"),
  "total" INTEGER,
  "placed-on-date-time" TEXT,
  "shipped-on-date" TEXT
);

CREATE INDEX "orders-customer-id" ON "orders" ("customer-id");

CREATE TABLE "shipping-schedules"
(
  "shipping-schedule-id" INTEGER PRIMARY KEY AUTOINCREMENT,
  "day-of-week" INTEGER,
  "time" TEXT,
  "is-refrigerated" INTEGER,
  "maximum-weight" REAL
);

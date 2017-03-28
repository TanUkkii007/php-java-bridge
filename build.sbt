val phpVersion = 5.6

val commonSettings = Seq(
  organization := "com.github.TanUkkii007",
  version := s"0.0.1-php$phpVersion",
  scalaVersion := "2.12.1",
  scalacOptions ++= Seq("-feature", "-deprecation", "-unchecked", "-encoding", "UTF-8", "-language:implicitConversions", "-language:postfixOps")
)

lazy val root = (project in file("."))
  .aggregate(`php-java-bridge-core`, `php-scala-bridge`)

lazy val `php-java-bridge-core` = (project in file("php-java-bridge-core"))
  .settings(commonSettings)
  .settings(
    libraryDependencies ++= Seq(
      "junit" % "junit" % "4.12" % "test",
      "com.novocode" % "junit-interface" % "0.11" % "test",
      "org.scalatest" %% "scalatest" % "3.0.1" % "test"
    ),
    testOptions += Tests.Argument(TestFrameworks.JUnit, "-v", "-a"),
    fork in Test := true,
    parallelExecution in Test := false,
    unmanagedResourceDirectories in Compile ++= Seq(baseDirectory.value / ".." / "out")
  )

lazy val `php-scala-bridge` = (project in file("php-scala-bridge"))
  .settings(commonSettings)
  .dependsOn(`php-java-bridge-core`)